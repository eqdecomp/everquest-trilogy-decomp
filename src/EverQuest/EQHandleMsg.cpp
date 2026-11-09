// EXTERNAL: TRILOGY 0x005218BB ??3@YAXPAX@Z
// EXTERNAL: TRILOGY 0x005218C6 ??2@YAPAXI@Z
// EXTERNAL: TRILOGY 0x005218BB ??3@YAXPAX@Z
// EXTERNAL: TRILOGY 0x005218C6 ??2@YAPAXI@Z
// EQHandleMsg.cpp - message handlers called from CEverQuest::HandleWorldMessage (0x00496743) and their
// helpers: the EverQuest/EQHandleMsg subsystem of the queue (call-graph labels, no Mac source file).

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "trilogy/common.h"
#include "trilogy/character.h"
#include "trilogy/player.h"
#include "trilogy/net_structs.h"
#include "trilogy/item.h"
#include "trilogy/globals.h"
#include "trilogy/display.h"
#include "trilogy/everquest.h"
#include "trilogy/eqhandlemsg.h"
#include "trilogy/layout/eq_main.h"
#include "trilogy/layout/eq_requestor.h"

// Globals used by msg_group_update_guess (each has the type of its uses there).
#define g_group_names_guess         EQ_ARRAY(char, 0x005FE8BD)      // 5 x 0x1E: member names
#define g_group_slot_used_guess     EQ_ARRAY(u8, 0x005FE8B8)        // 5 flags: name slot in use
#define g_everquest_guess           EQ_GLOBAL(void *, 0x006EFE2C)   // CEverQuest as void* (pinstCEverQuest_guess is typed)
#define g_pending_name_guess        EQ_ARRAY(char, 0x0055E088)      // "PENDING"
#define g_msg_formed_guess          EQ_ARRAY(char, 0x0055E06C)      // "..."
#define g_msg_joined_guess          EQ_ARRAY(char, 0x0055DFF4)      // "..."
#define g_msg_cannot_join_guess     EQ_ARRAY(char, 0x0055E0B0)      // "..."
#define g_msg_removed_guess         EQ_ARRAY(char, 0x0055E044)      // "..."
#define g_msg_disbanded_guess       EQ_ARRAY(char, 0x0055E090)      // "..."
#define g_fmt_member_joined_guess   EQ_ARRAY(char, 0x0055E028)      // "..."
#define g_fmt_member_left_guess     EQ_ARRAY(char, 0x0055E010)      // "..."

// Owner of the folded-stub object at +4 (class unknown; only the pointer at +4 is used here).
class EQStubOwner_guess
{
public:
    int unknown0_guess;                     // 0x00
    EQFoldedStubs_guess* stubs_guess;       // 0x04
    void ForwardToNop_guess(int arg);
};

// FUNCTION: TRILOGY 0x004019ec
// __thiscall. Forwards its argument to Nop_guess on the object at +4.
void EQStubOwner_guess::ForwardToNop_guess(int arg)
{
    stubs_guess->Nop_guess(arg);
}

// Minimal vtable layout for the message targets reached by DispatchUIEvent_guess.
class EQMessageTarget_guess
{
public:
    virtual void slot00_guess(void);
    virtual void slot04_guess(void);
    virtual void slot08_guess(void);
    virtual void slot0c_guess(void);
    virtual void slot10_guess(void);
    virtual char slot14_guess(void);
    virtual char slot18_guess(float* point);
    virtual char slot1c_guess(void);
    virtual void slot20_guess(void);
    virtual void slot24_guess(void);
    virtual void slot28_guess(void);
    virtual void slot2c_guess(void);
    virtual void slot30_guess(void);
    virtual void slot34_guess(int* message);
    virtual void slot38_guess(void);
    virtual void slot3c_guess(float* point);
};

/* EQMessageEvent_guess: the actor layout is EQMessageEvent_guess (generated: trilogy/layout/ui_input_device.h); the old local view's members map by offset. */
#include "trilogy/layout/ui_input_device.h"

// FUNCTION: TRILOGY 0x00401a1f
// Routes one queued input event to the UI: hover enter/leave notices, window dragging and click focus changes.
void __fastcall DispatchUIEvent_guess(char* self, int edx_unused, int* event)
{
    float* eventPoint;
    float targetY;
    float eventY;
    char cVar4;
    int* obj;
    int tmp;
    EQMessageEvent_guess notifyEvent;
    float dragPos[4];

    notifyEvent.x_guess = 0.0f;
    tmp = *event;
    notifyEvent.y_guess = 0.0f;
    notifyEvent.type_guess = 0x18;
    if (tmp == 0x10)
    {
        obj = (int*)EQ_METHOD(1, int, 0x00402C6E, (void*, float*), *(void**)self)((float*)(event + 1));
        if (*(void**)(self + 0x10) != 0)
        {
            dragPos[0] = ((UIInputDispatcher_guess*)self)->dragOffsetX_guess + *(float*)(event + 1);
            dragPos[1] = ((UIInputDispatcher_guess*)self)->dragOffsetY_guess + *(float*)(event + 2);
            dragPos[2] = dragPos[0];
            dragPos[3] = dragPos[1];
            ((EQMessageTarget_guess*)*(void**)(self + 0x10))->slot3c_guess(dragPos);
        }
        if (obj != (int*)*(void**)(self + 8))
        {
            if (*(void**)(self + 8) != 0)
            {
                notifyEvent.type_guess = 0x12;
                notifyEvent.x_guess = (float)(int)*(float*)(event + 1);
                notifyEvent.y_guess = (float)(int)*(float*)(event + 2);
                ((EQMessageTarget_guess*)*(void**)(self + 8))->slot34_guess((int*)&notifyEvent);
            }
            if (obj != 0)
            {
                notifyEvent.type_guess = 0x11;
                notifyEvent.x_guess = (float)(int)*(float*)(event + 1);
                notifyEvent.y_guess = (float)(int)*(float*)(event + 2);
                ((EQMessageTarget_guess*)obj)->slot34_guess((int*)&notifyEvent);
            }
            *(void**)(self + 8) = obj;
        }
        obj = (int*)*(void**)(self + 8);
    }
    else
    {
        if (tmp < 0x10)
        {
            eventPoint = (float*)(event + 1);
            tmp = EQ_METHOD(1, int, 0x00402C6E, (void*, float*), *(void**)self)(eventPoint);
            if ((tmp != 0) && (*(int*)(tmp + 8) == 0))
            {
                EQ_METHOD(1, void, 0x0040317D, (void*, int*), *(void**)self)(event);
            }
            if (*event == 0)
            {
                cVar4 = ((EQMessageTarget_guess*)*(void**)(self + 8))->slot18_guess(eventPoint);
                if (cVar4 != '\0')
                {
                    tmp = (int)*(void**)(self + 8);
                    ((UIInputDispatcher_guess*)self)->dragOffsetX_guess = *(float*)(tmp + 0x34) - *eventPoint;
                    targetY = *(float*)(tmp + 0x38);
                    eventY = *(float*)(event + 2);
                    *(void**)(self + 0x10) = (void*)tmp;
                    ((UIInputDispatcher_guess*)self)->dragOffsetY_guess = targetY - eventY;
                }
                cVar4 = ((EQMessageTarget_guess*)*(void**)(self + 8))->slot1c_guess();
                obj = (int*)*(void**)(self + 0xc);
                if (cVar4 == '\0')
                {
                    if (obj != 0)
                    {
                        ((EQMessageTarget_guess*)obj)->slot24_guess();
                        *(void**)(self + 0xc) = 0;
                    }
                }
                else
                {
                    if (obj != 0)
                    {
                        ((EQMessageTarget_guess*)obj)->slot24_guess();
                    }
                    *(void**)(self + 0xc) = *(void**)(self + 8);
                    ((EQMessageTarget_guess*)*(void**)(self + 8))->slot20_guess();
                }
            }
            else if (*event == 1)
            {
                *(void**)(self + 0x10) = 0;
            }
            if (*(void**)(self + 8) != 0)
            {
                ((EQMessageTarget_guess*)*(void**)(self + 8))->slot34_guess(event);
                return;
            }
            EQ_METHOD(1, void, 0x0040317D, (void*, int*), *(void**)self)(event);
            goto forward;
        }
        if ((0x15 < tmp) || (*(void**)(self + 0xc) == 0))
            goto forward;
        cVar4 = ((EQMessageTarget_guess*)*(void**)(self + 0xc))->slot14_guess();
        if (cVar4 == '\0')
        {
            *(void**)(self + 0xc) = 0;
            goto forward;
        }
        obj = (int*)*(void**)(self + 0xc);
    }
    ((EQMessageTarget_guess*)obj)->slot34_guess(event);
forward:
    EQ_METHOD(1, void, 0x0040240F, (void*, int*), ((UIInputDispatcher_guess*)self)->forwardTarget_guess)(event);
    return;
}

// FUNCTION: TRILOGY 0x00401fe8
// Dispatches queued mouse and device input to client UI controls.
void __fastcall ProcessQueuedValues_guess(void* self, int edx_unused, int unused)
{
    u32 value;
    int iterationCount;
    void* queue;

    EQ_METHOD(0, void, 0x00403ADB, (void*), *(void**)((char*)self + 0x0c))();
    EQ_METHOD(0, void, 0x004021B0, (void*), *(void**)((char*)self + 0x10))();
    iterationCount = 0x20;
    do
    {
        queue = *(void**)((char*)self + 0x0c);
        value = EQ_METHOD(0, u32, 0x00402099, (void*), (char*)queue + 0x0c)();
        if (value != 0)
        {
            EQ_METHOD(1, void, 0x00401A1F, (void*, u32), *(void**)((char*)self + 4))(value);
        }
        queue = *(void**)((char*)self + 0x10);
        value = EQ_METHOD(0, u32, 0x00402099, (void*), (char*)queue + 0x0c)();
        if (value != 0)
        {
            EQ_METHOD(1, void, 0x00401A1F, (void*, u32), *(void**)((char*)self + 4))(value);
        }
        iterationCount = iterationCount - 1;
    } while (iterationCount != 0);
}

// Node of a singly linked FIFO queue whose nodes are recycled through a free list.
/* EQQueueNode_guess: the actor layout is EQQueueNode_guess (generated: trilogy/layout/ui_input_device.h); the old local view's members map by offset. */
#include "trilogy/layout/ui_input_device.h"
#include <stdlib.h>
#include <time.h>

// Debug output and message sending by name, like the CRT calls: a direct call has its arguments popped
// right after it, as in the client; EQ_CDECL's constant function pointer lets VC6 merge neighbouring pops.
#undef LogMessage_guess
// EXTERNAL: TRILOGY 0x004C1703 DbgLog_guess
int __cdecl DbgLog_guess(const char* format, ...);
#define LogMessage_guess DbgLog_guess
#undef send_message_guess
// EXTERNAL: TRILOGY 0x004E1C45 send_message_guess
void __cdecl send_message_guess(void** connection, u32 opcode, void* data, u32 size, int reliable);

// Queue with a node free list (class unknown; offsets from the binary).
class EQNodeQueue_guess
{
public:
    int unknown0_guess;                     // 0x00
    EQQueueNode_guess* head_guess;          // 0x04
    EQQueueNode_guess* tail_guess;          // 0x08
    EQQueueNode_guess* free_guess;          // 0x0C
    char unknown10_guess[0x204];            // 0x10
    int count_guess;                        // 0x214
    u32 Pop_guess(void);
};

// FUNCTION: TRILOGY 0x00402099
// __thiscall. Pops the head node, recycles it onto the free list and returns its value; 0 when empty.
u32 EQNodeQueue_guess::Pop_guess(void)
{
    EQQueueNode_guess* node = head_guess;
    if (node != 0)
    {
        EQQueueNode_guess* next = node->next_guess;
        count_guess--;
        head_guess = next;
        if (count_guess == 0)
        {
            tail_guess = 0;
            head_guess = 0;
        }
        node->next_guess = free_guess;
        free_guess = node;
        return node->value_guess;
    }
    return 0;
}

// The queue owner layout used by AppendValue_guess; the original type is unknown.
class EQMsgQueueAppendOwner_guess
{
public:
    int capacity_guess;                      // 0x00
    int* head_guess;                          // 0x04
    int* tail_guess;                          // 0x08
    int* free_guess;                          // 0x0C
    int unknown10_guess[0x81];                // 0x10
    int count_guess;                          // 0x214

    void AppendValue_guess(int value);
};

// FUNCTION: TRILOGY 0x004021b0
// Collects pending input-device events for later client handling.
void __fastcall PollDeviceData_guess(void* self)
{
    int hr;
    u32 eventIndex;
    u32 eventCount;

    eventCount = 0x20;
    hr = ((int (__stdcall *)(void*, u32, void*, u32*, u32))
        (*(u32*)(*(u32*)(*(u32*)((char*)self + 4)) + 0x28)))
        (*(void**)((char*)self + 4), 0x10,
         *(void**)((char*)self + 8), &eventCount, 0);
    if (hr < 0)
    {
        hr = ((int (__stdcall *)(void*))
            (*(u32*)(*(u32*)(*(u32*)((char*)self + 4)) + 0x1c)))
            (*(void**)((char*)self + 4));
        if (hr >= 0)
        {
            ((int (__stdcall *)(void*, u32, void*, u32*, u32))
                (*(u32*)(*(u32*)(*(u32*)((char*)self + 4)) + 0x28)))
                (*(void**)((char*)self + 4), 0x10,
                 *(void**)((char*)self + 8), &eventCount, 0);
        }
        else
        {
            eventCount = 0;
        }
    }
    eventIndex = 0;
    if (eventCount > eventIndex)
    {
        do
        {
            u32* event;

            event = (u32*)operator new(0x14);
            if (event != (u32*)0)
            {
                *event = 0x18;
                *(float*)((char*)event + 4) = 0.0f;
                *(float*)((char*)event + 8) = 0.0f;
            }
            else
            {
                event = (u32*)0;
            }
            ((EQMsgQueueAppendOwner_guess*)((char*)self + 0x0c))
                ->AppendValue_guess((int)event);
            eventIndex = eventIndex + 1;
        } while (eventIndex < eventCount);
    }
    return;
}

// FUNCTION: TRILOGY 0x0040229e
// Adds one pending client input record to a reusable input queue.
void EQMsgQueueAppendOwner_guess::AppendValue_guess(int value)
{
    int* node;

    node = free_guess;
    if (node == 0)
    {
        EQ_METHOD(1, void, 0x0040233C, (void*, int), this)(capacity_guess);
        capacity_guess = (capacity_guess * 2);
        node = free_guess;
    }
    free_guess = (int*)*(int*)((char*)node + 4);
    if (tail_guess != 0)
    {
        *(int*)((char*)tail_guess + 4) = (int)node;
    }
    if (head_guess == 0)
    {
        head_guess = node;
    }
    memcpy(node, &value, 4);
    *(int*)((char*)node + 4) = 0;
    count_guess++;
    tail_guess = node;
}

// An 8-byte entry constructed by the compiler's array-new helper.
class EQMsgArrayEntry_guess
{
public:
    int unknown0_guess;                     // 0x00
    int* next_guess;                        // 0x04
    EQMsgArrayEntry_guess();
    ~EQMsgArrayEntry_guess();
};

// The owner fields used by AllocateEntryBlock_guess; the original class name is not known.
class EQMsgArrayOwner_guess
{
public:
    int unknown00_guess[3];                 // 0x00
    int* head_guess;                        // 0x0C
    int* entries_guess[128];                // 0x10
    int entryCount_guess;                   // 0x210

    void AllocateEntryBlock_guess(int count);
};

// FUNCTION: TRILOGY 0x0040233c
// Expands storage available for the client's queued input records.
void EQMsgArrayOwner_guess::AllocateEntryBlock_guess(int count)
{
    EQMsgArrayEntry_guess* entries;
    int* link;
    unsigned int remaining;

    entries = new EQMsgArrayEntry_guess[count];
    memset(entries, 0, count << 2);
    entries[count - 1].next_guess = head_guess;
    head_guess = (int*)entries;
    entries_guess[entryCount_guess] = (int*)entries;
    remaining = count + -2;
    entryCount_guess = entryCount_guess + 1;
    if (remaining > 0)
    {
        link = (int*)entries + 1;
        do
        {
            *link = (int)(link + 1);
            link = link + 2;
            remaining = remaining + -1;
        } while (remaining != 0);
    }
}

// FUNCTION: TRILOGY 0x004023d3
// The game purpose of destroying a reusable message queue entry is unclear.
EQMsgArrayEntry_guess::~EQMsgArrayEntry_guess()
{
    next_guess = 0;
}

// FUNCTION: TRILOGY 0x004023d8
// The game purpose of creating a reusable message queue entry is unclear.
EQMsgArrayEntry_guess::EQMsgArrayEntry_guess()
{
    next_guess = 0;
}

// FUNCTION: TRILOGY 0x0040240f
// Its game purpose is unclear; UI and player code use it as a callback.
// ORIGINAL FILE: unknown (subsystem from the call graph)
// An empty __thiscall method with one stack argument (`ret 4`). The linker folded identical empty methods of
// several classes into this one: HandleWorldMessage calls it on the EQ_PC object, the old UI on windows.
void EQFoldedStubs_guess::Nop_guess(int unused)
{
}

// FUNCTION: TRILOGY 0x00402ae3
// Input system tick: resets the input object, then runs its queued mouse and device input processing.
// ORIGINAL FILE: unknown (subsystem from the call graph)
// __fastcall (`this` in ECX, no stack argument, callee pops nothing). Resets the window through 0x0040101D on the
// object at +0x1C, saves +0x20 into +0x24, stores the global at 0x0058FB50 into +0x20 and passes it to 0x00401FE8
// (ECX = +0x08), then forwards it to 0x004019EC (ECX = +0x0C) and to Nop_guess (ECX = +0x10).
void __fastcall TickInputSystem_guess(int owner)
{
    u32 tickValue;

    EQ_METHOD(0, void, 0x0040101D, (int), *(int*)(owner + 0x1c))();
    *(int*)(owner + 0x24) = *(int*)(owner + 0x20);
    tickValue = EQ_GLOBAL(u32, 0x0058FB50);
    *(u32*)(owner + 0x20) = tickValue;
    EQ_METHOD(1, void, 0x00401FE8, (int, u32), *(int*)(owner + 0x08))(tickValue);
    EQ_METHOD(1, void, 0x004019EC, (int, int), *(int*)(owner + 0x0c))(*(int*)(owner + 0x20));
    EQ_METHOD(1, void, 0x0040240F, (int, int), *(int*)(owner + 0x10))(*(int*)(owner + 0x20));
    return;
}

// A node with an array of child nodes from begin (+0x0C) to end (+0x10) and a hit test in vtable slot 10
// (+0x28) taking one argument (class unknown; offsets from the binary).
class EQHitNode_guess
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
    virtual char HitTest_guess(int arg);    // vtable +0x28
    char unknown4_guess[8];                 // 0x04
    EQHitNode_guess** begin_guess;          // 0x0C
    EQHitNode_guess** end_guess;            // 0x10
    EQHitNode_guess* FindHit_guess(int arg);
};

// Holder of a hit-test root node at +0x14 (class unknown; only that pointer is used here).
class EQHitRoot_guess
{
public:
    char unknown0_guess[0x14];              // 0x00
    EQHitNode_guess* root_guess;            // 0x14
    EQHitNode_guess* FindHitInRoot_guess(int arg);
};

// FUNCTION: TRILOGY 0x00402c6e
// __thiscall. Runs FindHit_guess (0x00404115) on the node at +0x14; 0 when there is none.
EQHitNode_guess* EQHitRoot_guess::FindHitInRoot_guess(int arg)
{
    EQHitNode_guess* result = 0;
    if (root_guess != 0)
        result = EQ_METHOD(1, EQHitNode_guess*, 0x00404115, (EQHitNode_guess*, int), root_guess)(arg);
    return result;
}

// FUNCTION: TRILOGY 0x00403adb
// Turns buffered DirectInput mouse data into queued input events: cursor moves, button down/double-click/up.
// @early-stop: frame size, coordinate clamp/x87 spills, record address operands, and the EQ_THISCALL EDX shim differ; the mutants that survive are equivalent.
void __fastcall PollMouseDeviceData_guess(void* mouseInput)
{
    int tmp;
    u32* event;
    int* dataRecord;
    u32 eventType;
    int temp;
    int* lastClickTime;
    int recordOffset = 0;
    u32 eventIndex;
    u32 eventCount;
    int* self = (int*)mouseInput;
    void* device;

    eventCount = 0x80;
    device = (void*)self[1];
    tmp = ((int (__stdcall *)(void*, int, void*, u32*, int))(*(void***)device)[10])
        (device, 0x10, (void*)self[2], &eventCount, 0);
    if (tmp < 0)
    {
        device = (void*)self[1];
        tmp = ((int (__stdcall *)(void*))(*(void***)device)[7])(device);
        if (tmp >= 0)
        {
            device = (void*)self[1];
            ((int (__stdcall *)(void*, int, void*, u32*, int))(*(void***)device)[10])
                (device, 0x10, (void*)self[2], &eventCount, 0);
        }
        else
        {
            eventCount = 0;
        }
    }
    eventIndex = 0;
    if (eventCount > 0)
    {
        for (; eventIndex < eventCount; ++eventIndex, recordOffset += 0x10)
        {
            event = (u32*)operator new(0x14);
            if (event)
            {
                *event = 0x18;
                ((float*)event)[1] = 0.0f;
                ((float*)event)[2] = 0.0f;
            }
            else
            {
                event = (u32*)0;
            }
            tmp = self[0x228 / 4];
            ((float*)event)[1] = (float)self[0x224 / 4];
            ((float*)event)[2] = (float)tmp;
            dataRecord = (int*)((char*)self[2] + recordOffset);
            switch (*dataRecord)
            {
            case 0:
            {
                tmp = dataRecord[1] + self[0x224 / 4];
                self[0x224 / 4] = tmp;
                if (tmp < 0)
                    tmp = 0;
                self[0x224 / 4] = tmp;
                if (self[0x22c / 4] < tmp)
                    tmp = self[0x22c / 4];
                self[0x224 / 4] = tmp;
                ((float*)event)[1] = (float)tmp;
                temp = self[0x228 / 4];
                ((float*)event)[2] = (float)temp;
                *event = 0x10;
                break;
            }
            case 4:
            {
                temp = dataRecord[1] + self[0x228 / 4];
                self[0x228 / 4] = temp;
                if (temp < 0)
                    temp = 0;
                self[0x228 / 4] = temp;
                if (self[0x22c / 4] < temp)
                    temp = self[0x22c / 4];
                self[0x228 / 4] = temp;
                ((float*)event)[1] = (float)self[0x224 / 4];
                ((float*)event)[2] = (float)temp;
                *event = 0x10;
                break;
            }
            case 12:
            {
                if (dataRecord[1] != 0)
                {
                    lastClickTime = (int*)((char*)self + 0x238);
                    eventType = ((u32)(dataRecord[2] - *lastClickTime) < *(u32*)((char*)self + 0x234)) ? 3 : 0;
                    *event = eventType;
                    *lastClickTime = *(int*)((char*)self[2] + recordOffset + 8);
                }
                else
                    *event = 1;
                break;
            }
            case 13:
            {
                if (dataRecord[1] != 0)
                {
                    lastClickTime = (int*)((char*)self + 0x23c);
                    eventType = (((u32)(dataRecord[2] - *lastClickTime) < *(u32*)((char*)self + 0x234)) ? 3 : 0) + 4;
                    *event = eventType;
                    *lastClickTime = *(int*)((char*)self[2] + recordOffset + 8);
                }
                else
                    *event = 1;
                break;
            }
            case 14:
            {
                if (dataRecord[1] != 0)
                {
                    lastClickTime = (int*)((char*)self + 0x240);
                    eventType = (((u32)(dataRecord[2] - *lastClickTime) < *(u32*)((char*)self + 0x234)) ? 3 : 0) + 8;
                    *event = eventType;
                    *lastClickTime = *(int*)((char*)self[2] + recordOffset + 8);
                }
                else
                    *event = 1;
                break;
            }
            case 15:
            {
                if (dataRecord[1] != 0)
                {
                    lastClickTime = (int*)((char*)self + 0x244);
                    eventType = (((u32)(dataRecord[2] - *lastClickTime) < *(u32*)((char*)self + 0x234)) ? 3 : 0) + 0xc;
                    *event = eventType;
                    *lastClickTime = *(int*)((char*)self[2] + recordOffset + 8);
                }
                else
                    *event = 1;
                break;
            }
            default:
                break;
            }
            EQ_METHOD(1, void, 0x0040229E, (void*, int), (char*)self + 0xc)((int)event);
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x00404115
// Identifies the specific nested UI control under the tested pointer location.
// __thiscall. Returns the deepest node whose hit test passes for `arg`: descends into the first child that
// passes; returns this when no child passes (or the descent returns 0).
EQHitNode_guess* EQHitNode_guess::FindHit_guess(int arg)
{
    EQHitNode_guess** p;

    for (p = begin_guess; p != end_guess; p++)
    {
        EQHitNode_guess* child = *p;
        if (child->HitTest_guess(arg))
        {
            EQHitNode_guess* found = child->FindHit_guess(arg);
            if (found != 0)
                return found;
            break;
        }
    }
    return this;
}

// FUNCTION: TRILOGY 0x00480e87
// The game purpose of this EQ_PC callback during world-message handling is unclear.
// ORIGINAL FILE: unknown (subsystem from the call graph)
// A __thiscall method with three stack arguments that returns 0 (`ret 0xc`); also a vtable slot at
// 0x00535818 and 0x00535820. HandleWorldMessage calls it on the EQ_PC object.
int EQFoldedStubs_guess::ReturnZero_guess(int unused1, int unused2, int unused3)
{
    return 0;
}

// FUNCTION: TRILOGY 0x0049100c
// Expands compressed item data and world-message payloads before their handlers use them.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name DecompressBuffer, low)
// __cdecl. Inflates `in` (inLen bytes) into `out` (outSize bytes) in one Z_FINISH call through the global
// z_stream at 0x00596378 (zalloc/zfree/opaque as they are there); returns total_out.
u32 DecompressBuffer_guess(u8* in, u8* out, u32 inLen, u32 outSize)
{
    s32 total;

    {
        struct ZStream_guess *temp3 = g_inflate_stream_guess;
        struct ZStream_guess *temp = temp3;
        temp3->next_in = in;
        temp3->avail_in = inLen;
        g_inflate_stream_guess->total_in = 0;
        temp3->next_out = out;
        temp->avail_out = outSize;
        temp3->total_out = 0;
    }
    g_inflate_stream_guess->data_type = 0;
    inflateInit2__guess(g_inflate_stream_guess, 15, EQ_ARRAY(char, 0x0055DD70), 0x38);
    inflate_guess(g_inflate_stream_guess, 4);
    total = g_inflate_stream_guess->total_out;
    inflateEnd_guess(g_inflate_stream_guess);
    return total;
}

// FUNCTION: TRILOGY 0x00491137
// Extracts a displayable line from a multiline server text message.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name ParseNextLine, low)
// __cdecl. Copies the next line of `text`, from *pos, into `line` (at most lineSize - 1 characters, '@'
// becomes an apostrophe) and advances *pos past it and its line break. False when *pos is already at the
// end, when the text ends inside the line or right after its line break; otherwise true while text
// remains after *pos. Only the first of a \r\n pair is consumed.
bool ParseNextLine_guess(char* text, char* line, u32* pos, int lineSize)
{
    u32 n = 0;
    int state = 0;
    u32 len;
    char c;

    len = strlen(text);
    *line = '\0';
    if (*pos >= len)
        return false;
    do
    {
        c = text[(*pos)++];
        if (c == '\0')
        {
            state = 2;
            break;
        }
        if (c == '\n' || c == '\r')
        {
            state = 1;
            if (*pos < len)
                break;
            state = 2;
        }
        else
        {
            if (c == '@')
                c = '\'';
            line[n++] = c;
            if (n >= (u32)(lineSize - 1))
                state = 1;
            if (*pos >= len)
            {
                state = 2;
                break;
            }
        }
    } while (state == 0);
    line[n] = '\0';
    if (state == 2)
        return false;
    return text[*pos] != '\0';
}

// FUNCTION: TRILOGY 0x004911d3
// Prepares an incoming text prefix for display; its exact message role is unclear.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQHandleMsg.cpp (Mac: stripfirstword, high)
// __cdecl. 0 for text starting "[<digit>" or of 3 characters or less. Otherwise turns '_' into spaces up to
// the first space and, when the character two before that space is a digit, removes those two characters
// (a two-digit name suffix) by moving the rest of the text left; returns 1.
int stripfirstword(char* text)
{
    u32 i = 0;
    int done = 0;
    u32 space;
    u32 len;
    u32 k;

    if (text[0] == '[' && isdigit(text[1]))
        return 0;
    len = strlen(text);
    if (len < 4)
        return 0;
    space = (u32)-1;
    do
    {
        if (text[i] == ' ')
        {
            space = i;
            done = 1;
        }
        else if (text[i] == '_')
        {
            text[i] = ' ';
        }
        i++;
        if (i >= len)
            done = 1;
    } while (!done);
    if ((int)space > 2 && isdigit(text[space - 2]))
    {
        for (k = space - 2; k < len - 2; k++)
            text[k] = text[k + 2];
        text[len - 2] = '\0';
    }
    return 1;
}

// FUNCTION: TRILOGY 0x00491272
// Answers a guild invite or rank change: opens the accept dialog, auto-declines, or reports a demotion.
// @early-stop: register allocation, comparison ordering, and call cleanup still differ.
void HandleGuildInvite_guess(int conn, int msgOpcode, char* inviteData)
{
    short myGuildId;
    int inviteRank;
    u32 newRank;
    int compare_result;
    char* invitee_name;
    char* guildName;
    char* chatText;
    char chatBuffer[128];
    struct InvitePacket_guess
    {
        char recipient[30];
        char profile_name[30];
        int status;
        short guild_id;
        short padding;
    } reply;
    void* invite_state;
    typedef void (__fastcall *InviteCallback_guess_t)(void*, int);
    InviteCallback_guess_t invite_callback;

    if (EQ_GLOBAL(int, 0x006E4E44) == 0)
        return;
    compare_result = _strcmpi((const char*)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8), inviteData);
    if (compare_result != 0)
        return;
    invitee_name = inviteData + 0x1e;
    strcpy(reply.recipient, invitee_name);
    strcpy(reply.profile_name, (const char*)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
    reply.guild_id = *(short*)(inviteData + 0x3c);
    myGuildId = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_id;
    if ((myGuildId == reply.guild_id) || (EQ_GLOBAL(int, 0x006B686C) != 0))
    {
        if ((myGuildId < 0) || (myGuildId == reply.guild_id))
        {
            if (*(int*)((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x10) == 0)
            {
                if ((myGuildId == -1) &&
                    (*(int*)(inviteData + 0x40) >= 0) &&
                    (*(int*)(inviteData + 0x40) <= 2))
                {
                    pinstEQ_Requestor_guess->requestAction_guess = 0;
                    *(char*)((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x174) = *(char*)(inviteData + 0x40);
                    pinstEQ_Requestor_guess->requestId_guess = *(short*)(inviteData + 0x3c);
                    strcpy((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x5b9, invitee_name);
                    invite_state = EQ_GLOBAL(void*, 0x006E4E20);
                    invite_callback = (InviteCallback_guess_t)
                        (*(int*)(*(int*)invite_state + 4));
                    invite_callback(invite_state, 0);
                    pinstEQ_Requestor_guess->timeout_guess =
                        *(int*)((char*)g_display_guess + 0xbc) + 10000;
                    inviteRank = *(int*)(inviteData + 0x40);
                    switch (inviteRank)
                    {
                    case 0:
                        guildName = EQ_CDECL(char*, 0x004E00F6, (short))((short)*(short*)(inviteData + 0x3c));
                        sprintf((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x1b9,
                             EQ_ARRAY(char, 0x0055DE48), invitee_name, guildName);
                        break;
                    case 1:
                        guildName = EQ_CDECL(char*, 0x004E00F6, (short))((short)*(short*)(inviteData + 0x3c));
                        sprintf((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x1b9,
                             EQ_ARRAY(char, 0x0055DE0C), invitee_name, guildName);
                        break;
                    case 2:
                        guildName = EQ_CDECL(char*, 0x004E00F6, (short))((short)*(short*)(inviteData + 0x3c));
                        sprintf((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x1b9,
                             EQ_ARRAY(char, 0x0055DE80), invitee_name, guildName);
                        break;
                    default:
                        return;
                    }
                    return;
                }
                if (myGuildId != reply.guild_id)
                    return;
                newRank = *(u32*)(inviteData + 0x40);
                if (newRank == (u8)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_rank)
                {
                    reply.status = newRank + 10;
                }
                else
                {
                    if ((int)(u32)(u8)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_rank <= (int)newRank)
                    {
                        pinstEQ_Requestor_guess->requestAction_guess = 0;
                        *(char*)((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x174) = *(char*)(inviteData + 0x40);
                        pinstEQ_Requestor_guess->requestId_guess = *(short*)(inviteData + 0x3c);
                        strcpy((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x5b9, invitee_name);
                        invite_state = EQ_GLOBAL(void*, 0x006E4E20);
                        invite_callback = (InviteCallback_guess_t)
                            (*(int*)(*(int*)invite_state + 4));
                        invite_callback(invite_state, 0);
                        pinstEQ_Requestor_guess->timeout_guess =
                            *(int*)((char*)g_display_guess + 0xbc) + 10000;
                        switch (*(int*)(inviteData + 0x40))
                        {
                        case 1:
                            sprintf((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x1b9,
                                 EQ_ARRAY(char, 0x0055DD78), invitee_name);
                            break;
                        case 2:
                            sprintf((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x1b9,
                                 EQ_ARRAY(char, 0x0055DDAC), invitee_name);
                            break;
                        default:
                            return;
                        }
                        return;
                    }
                    if (newRank == 0)
                    {
                        sprintf(chatBuffer, EQ_ARRAY(char, 0x0055DDE0), invitee_name);
                        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(chatBuffer, 0x111);
                    }
                    reply.status = *(int*)(inviteData + 0x40);
                }
                send_message_guess(&EQ_GLOBAL(void*, 0x006B3194), 0x2118, &reply, 0x44, 1);
                return;
            }
            reply.status = 4;
        }
        else
        {
            reply.status = 3;
        }
        send_message_guess(&EQ_GLOBAL(void*, 0x006B3194), 0x2118, &reply, 0x44, 1);
        inviteRank = *(int*)(inviteData + 0x40);
        switch (inviteRank)
        {
        case 0:
            guildName = EQ_CDECL(char*, 0x004E00F6, (short))((short)*(short*)(inviteData + 0x3c));
            sprintf(chatBuffer, EQ_ARRAY(char, 0x0055DE48), invitee_name, guildName);
            break;
        case 1:
            guildName = EQ_CDECL(char*, 0x004E00F6, (short))((short)*(short*)(inviteData + 0x3c));
            sprintf(chatBuffer, EQ_ARRAY(char, 0x0055DE0C), invitee_name, guildName);
            break;
        case 2:
            guildName = EQ_CDECL(char*, 0x004E00F6, (short))((short)*(short*)(inviteData + 0x3c));
            sprintf(chatBuffer, EQ_ARRAY(char, 0x0055DE80), invitee_name, guildName);
            break;
        default:
            return;
        }
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(chatBuffer, 0x111);
        return;
    }
    else
    {
        reply.status = 5;
        send_message_guess(&EQ_GLOBAL(void*, 0x006B3194), 0x2118, &reply, 0x44, 1);
        inviteRank = *(int*)(inviteData + 0x40);
        switch (inviteRank)
        {
        case 0:
            guildName = EQ_CDECL(char*, 0x004E00F6, (short))((short)*(short*)(inviteData + 0x3c));
            chatText = EQ_ARRAY(char, 0x0055DF14);
            sprintf(chatBuffer, chatText, invitee_name, guildName);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(chatBuffer, 0x111);
            break;
        case 1:
                guildName = EQ_CDECL(char*, 0x004E00F6, (short))((short)*(short*)(inviteData + 0x3c));
                chatText = EQ_ARRAY(char, 0x0055DF5C);
                sprintf(chatBuffer, chatText, invitee_name, guildName);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(chatBuffer, 0x111);
            break;
        case 2:
                guildName = EQ_CDECL(char*, 0x004E00F6, (short))((short)*(short*)(inviteData + 0x3c));
                chatText = EQ_ARRAY(char, 0x0055DFA8);
                sprintf(chatBuffer, chatText, invitee_name, guildName);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(chatBuffer, 0x111);
            break;
        default:
            break;
        }
        chatText = EQ_ARRAY(char, 0x0055DEBC);
    }
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(chatText, 0x111);
    return;
}

// FUNCTION: TRILOGY 0x00491632
// Rebuilds the group member list and prints joined, left, formed, removed, disbanded or group-full lines.
// @early-stop: 345 instructions remain different in stack-local placement, clear-loop registers, and branch/call sequences.
// __cdecl, one stack argument (the group message). Sets +0xEB8 of the UI object (0x006E4E44) to 1, then does
// nothing while 0x006B63F8 is set or 0x006B6494 is not 0.
// A NULL message, or one whose 0xE0 is not 5, is a group update: the local profile's member names (6 x 48 bytes at
// 0x1078) are compared with the name table at 0x005FE8BD (5 x 30 bytes), which is rebuilt, and a join, leave,
// formed, removed or disbanded line is printed. 0xE0 == 5 prints "..." and clears the tables.
// Stack slots are shared as in the original: msg is also the loop index, a found flag and a name cursor;
// has_others is also the write cursor, removed_code a name cursor, count the loop counter.
void msg_group_update_guess(char* msg)
{
    char old_names[320];                    // 5 x 0x40: the name table before this update
    char chat_line[512];
    char selected_name[32];                 // name at the +0xB4 index, restored after the rebuild
    char* volatile old_slot;                // volatile: matches the original's stack slot
    u32* slot;
    char* removed_code;                     // 1 or 2 on the leave path; a name cursor in the reset branch
    int count;                              // 1 = old group had another member; a loop counter elsewhere
    u32* has_others;                        // 1 = group has another member; the write cursor elsewhere
    int cmp;
    int i;                                  // member offset (0x30 stride) or slot index, per loop
    int j;
    u32 player;
    char member_index;
    char* other;
    char* text;
    char* entry;
    u32* cur_slot;

    removed_code = NULL;
    pinstEQ_Main_guess->uiDirty_guess = 1;
    count = 0;
    has_others = NULL;
    if (g_offlineMode_guess == '\0' && DAT_006b6494 == 0)
    {
        if (msg == NULL ||
            (EQ_CDECL(void, 0x004A3050, (void*, void*))(EQ_GLOBAL(void*, 0x006E4DA8), msg),
             *(int*)(msg + 0xE0) != 5))
        {
            selected_name[0] = '\0';
            i = 0;
            do
            {
                if (g_pc_guess->group_members[0][i] != '\0' &&
                    _strcmpi(
                        g_pc_guess->group_members[0] + i, (const char*)g_pc_guess) != 0)
                {
                    has_others = (u32*)1;
                }
                i = i + 0x30;
            } while (i < 0x120);
            // save each old name, then clear its slot
            old_slot = old_names;
            msg = NULL;
            entry = g_group_names_guess;
            slot = g_death_state_guess;
            do
            {
                strcpy(old_slot, entry);
                if (*old_slot != '\0' &&
                    _strcmpi(
                        (const char*)g_pc_guess, old_slot) != 0)
                {
                    count = 1;
                }
                if ((int)msg == pinstEQ_Main_guess->groupSelected_guess)
                {
                    strcpy(selected_name, entry);
                }
                cur_slot = slot;
                old_slot = old_slot + 0x40;
                g_group_slot_used_guess[(int)msg] = 0;
                slot = slot + 1;
                *cur_slot = 0;
                *entry = '\0';
                msg = msg + 1;
                entry = entry + 0x1E;
            } while ((int)slot < 0x005FE968);   // end of g_group_players_guess
            i = 0;
            pinstEQ_Main_guess->groupSelected_guess = (char)0xFF;
            // no other member before: formed or joined
            if (count == 0)
            {
                if (has_others != NULL)
                {
                    if (g_name_buffer_guess[0] == '\0')
                    {
                        strcpy(g_name_buffer_guess, g_pending_name_guess);
                    }
                    i = strcmp(
                        g_name_buffer_guess, (const char*)g_pc_guess);
                    if (i == 0)
                    {
                        text = g_msg_formed_guess;
                    }
                    else
                    {
                        text = g_msg_joined_guess;
                    }
                    sprintf(chat_line, text);
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(chat_line, 0x111);
                    EQ_CDECL(void, 0x004A8A32, (void*, void*))(EQ_GLOBAL(void*, 0x006E4D8C), EQ_ARRAY(void*, 0x0055B254));
                    i = 0;
                    has_others = g_death_state_guess;
                    j = 0;
                    msg = g_group_names_guess;
                    do
                    {
                        if (g_pc_guess->group_members[0][j] != '\0' &&
                            (cmp = strcmp(
                                g_pc_guess->group_members[0] + j, (const char*)g_pc_guess), cmp != 0))
                        {
                            strcpy(
                                msg, g_pc_guess->group_members[0] + j);
                            g_group_slot_used_guess[i] = 1;
                            player = (u32)EQ_METHOD(1, EQPlayer*, 0x004B1435, (void*, const char*), EQ_GLOBAL(void*, 0x006E4D58))(msg);
                            msg = msg + 0x1E;
                            i = i + 1;
                            *has_others = player;
                            has_others = has_others + 1;
                            if (0x005FE952 < (int)msg)
                            {
                                return;
                            }
                        }
                        j = j + 0x30;
                    } while (j < 0x120);
                }
            }
            // others before: diff the old and new members
            else
            {
                if (g_name_buffer_guess[0] != '\0')
                {
                    do
                    {
                        j = _strcmpi(
                            g_name_buffer_guess, g_pc_guess->group_members[0] + i);
                        if (j == 0)
                        {
                            removed_code = (char*)1;
                        }
                        i = i + 0x30;
                    } while (i < 0x120);
                    i = _strcmpi(
                        g_name_buffer_guess, (const char*)g_pc_guess);
                    if (i == 0)
                    {
                        removed_code = (char*)2;
                    }
                }
                i = 0;
                if (has_others == NULL)
                {
                    if (removed_code == (char*)1 || removed_code != (char*)2)
                    {
                        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(g_msg_removed_guess, 0x111);
                    }
                    else
                    {
                        // 0x2197 = OP_GroupDelete
                        send_message_guess(g_world_connection_guess, 0x2197, NULL, 0, 1);
                        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(g_msg_disbanded_guess, 0x111);
                    }
                    g_name_buffer_guess[0] = '\0';
                }
                else
                {
                    has_others = g_death_state_guess;
                    j = 0;
                    msg = g_group_names_guess;
                    do
                    {
                        if (g_pc_guess->group_members[0][j] != '\0' &&
                            (cmp = strcmp(
                                g_pc_guess->group_members[0] + j, (const char*)g_pc_guess), cmp != 0))
                        {
                            strcpy(
                                msg, g_pc_guess->group_members[0] + j);
                            g_group_slot_used_guess[i] = 1;
                            player = (u32)EQ_METHOD(1, EQPlayer*, 0x004B1435, (void*, const char*), EQ_GLOBAL(void*, 0x006E4D58))(msg);
                            msg = msg + 0x1E;
                            i = i + 1;
                            *has_others = player;
                            has_others = has_others + 1;
                            if (0x005FE952 < (int)msg)
                            {
                                break;
                            }
                        }
                        j = j + 0x30;
                    } while (j < 0x120);
                    text = g_group_names_guess;
                    do
                    {
                        msg = NULL;
                        if (*text != '\0')
                        {
                            other = old_names;
                            count = 5;
                            do
                            {
                                if (strcmp(text, other) == 0)
                                {
                                    msg = (char*)1;
                                }
                                other = other + 0x40;
                                count = count - 1;
                            } while (count != 0);
                            if (msg == NULL)
                            {
                                sprintf(
                                    chat_line, g_fmt_member_joined_guess, text);
                                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(chat_line, 0x111);
                            }
                        }
                        text = text + 0x1E;
                    } while ((int)text < 0x005FE953);
                    text = old_names;
                    count = 5;
                    do
                    {
                        msg = NULL;
                        if (*text != '\0')
                        {
                            other = g_group_names_guess;
                            do
                            {
                                if (strcmp(text, other) == 0)
                                {
                                    msg = (char*)1;
                                }
                                other = other + 0x1E;
                            } while ((int)other < 0x005FE953);
                            if (msg == NULL)
                            {
                                sprintf(
                                    chat_line, g_fmt_member_left_guess, text);
                                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(chat_line, 0x111);
                            }
                        }
                        text = text + 0x40;
                        count = count - 1;
                    } while (count != 0);
                    member_index = '\0';
                    text = g_group_names_guess;
                    do
                    {
                        if (selected_name[0] != '\0' &&
                            strcmp(selected_name, text) == 0)
                        {
                            pinstEQ_Main_guess->groupSelected_guess = member_index;
                        }
                        text = text + 0x1E;
                        member_index = member_index + 1;
                    } while ((int)text < 0x005FE953);
                }
            }
        }
        else
        {
            g_name_buffer_guess[0] = '\0';
            j = 0;
            i = 0;
            removed_code = g_group_names_guess;
            cur_slot = g_death_state_guess;
            do
            {
                g_pc_guess->group_members[0][i] = '\0';
                ((u32*)g_pc_guess->unknown0x1198)[j] = 0;
                if ((int)cur_slot < 0x005FE968)
                {
                    g_group_slot_used_guess[j] = 0;
                    *cur_slot = 0;
                    *removed_code = '\0';
                }
                removed_code = removed_code + 0x1E;
                cur_slot = cur_slot + 1;
                j = j + 1;
                i = i + 0x30;
            } while ((int)cur_slot < 0x005FE96C);
            if (g_eqmain_guess != NULL)
            {
                g_eqmain_guess[0xB5] = '\0';
                g_eqmain_guess[0xB4] = (char)0xFF;
                pinstEQ_Main_guess->uiDirty_guess = 1;
            }
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(g_msg_cannot_join_guess, 0x111);
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x00491b5d
// Applies a server relocation for a zone spawn and refreshes its visible position.
// ORIGINAL FILE: unknown (subsystem from the call graph)
// __cdecl, OP_MoveSpawn (MsgMoveSpawn). Puts the spawn at the position, stops it (speed and velocities 0),
// sets its actor's floor height from CDisplay::GetPlayerFloorHeight (probing from z + 1; -1e27 = no floor,
// left alone) and updates its actor.
void msg_move_spawn_guess(MsgMoveSpawn* msg)
{
    EQPlayer* player = EQPlayer_GetPlayerFromID_guess(g_player_top_guess, msg->spawn_id);
    float floor;

    if (player != NULL)
    {
        player->y = msg->y;
        player->x = msg->x;
        player->z = msg->z;
        player->speed = 0.0f;
        player->dy = 0.0f;
        player->dx = 0.0f;
        player->dz = 0.0f;
        player->delta_heading = 0.0f;
        floor = CDisplay_GetPlayerFloorHeight_guess(g_display_guess, player, player->y, player->x, player->z + 1.0f, 0);
        if (floor != -1e27f)
            player->actor_info->floor_z = floor;
        CDisplay_UpdatePlayerActor_guess(g_display_guess, player);
    }
}

// FUNCTION: TRILOGY 0x00491bf0
// Updates the petition queue and refreshes its list when the number of open petitions changes.
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
// __cdecl, one stack argument, returns 1. Petition-queue update from a 0x74-byte message (id at 0, rank at 4
// (signed), flag at 8, seq at 0xC (unsigned, 0 = unused), name at 0x10, kind at 0x50). The queue is 25 entries of
// 0x74 bytes at pinstCEverQuest+0x4C (id -1 = empty); +0xBA0 takes the message's 0x50 value and +0xBA4 counts the
// used entries. An entry with the message's id takes the message (its id becomes -1 when the flag is -1 or 1; a
// nameless message gets "..."). Then all 25 entries are saved as candidates (the message is a 26th one when
// no id matched) and each entry in turn takes the candidate that beats it (higher rank, or lower seq, or seq 0); the
// chosen candidate is used up (id -1). When the count changed and the window at g_stml_wnd_guess is in mode 1 (+0x8C),
// it is redrawn (vtable slots 2 and 1, then ParseSTML); that window's +0xB0 is 1 while this runs.
int msg_pq_update(int* petitionUpdate)
{
    int old_count;
    int off;
    int k;
    int best;
    int next;
    int* cand;
    bool found;
    int cand_buf[26][29];       // 0x000-0xB54: the 25 saved entries; 0xB54-0xBCC: the message (26th candidate)

    old_count = pinstCEverQuest_guess->petitionCount_guess;
    off = 0;
    if (EQ_GLOBAL(int*, 0x006E4DF4) != NULL)
        EQ_GLOBAL(int*, 0x006E4DF4)[0x2C] = 1;
    pinstCEverQuest_guess->petitionKind_guess = petitionUpdate[0x14];
    pinstCEverQuest_guess->petitionCount_guess = 0;
    cand_buf[25][0] = -1;
    if (*(char*)(petitionUpdate + 4) == '\0')
        strcpy((char*)(petitionUpdate + 4), EQ_ARRAY(char, 0x0055E0D8));
    found = false;
    do
    {
        if (*(int*)((char*)g_everquest_guess + off + 0x4C) == *petitionUpdate)
        {
            found = true;
            memcpy((char*)g_everquest_guess + off + 0x4C, petitionUpdate, 0x74);
            if (petitionUpdate[2] == -1 || petitionUpdate[2] == 1)
                *(int*)((char*)g_everquest_guess + off + 0x4C) = -1;
        }
        off = off + 0x74;
    } while (off < 0xB54);
    off = 0;
    do
    {
        memcpy((char*)cand_buf + off, (char*)g_everquest_guess + off + 0x4C, 0x74);
        next = off + 0x74;
        *(int*)((char*)g_everquest_guess + off + 0x4C) = -1;
        *(int*)((char*)g_everquest_guess + off + 0x50) = 0;
        *(int*)((char*)g_everquest_guess + off + 0x58) = 0;
        off = next;
    } while (off < 0xB54);
    if (!found)
    {
        memcpy(cand_buf[25], petitionUpdate, 0x74);
        if (petitionUpdate[2] == -1 || petitionUpdate[2] == 1)
            cand_buf[25][0] = -1;
    }
    off = 0;
    do
    {
        best = -1;
        k = 0;
        cand = (int*)cand_buf;
        do
        {
            if (-1 < cand[0] &&
                (*(int*)((char*)g_everquest_guess + off + 0x50) < cand[1] ||
                 (u32)cand[3] < *(u32*)((char*)g_everquest_guess + off + 0x58) ||
                 *(u32*)((char*)g_everquest_guess + off + 0x58) == 0))
            {
                memcpy((char*)g_everquest_guess + off + 0x4C, cand, 0x74);
                best = k;
            }
            k = k + 1;
            cand = cand + 0x1D;
        } while (k < 0x1A);
        if (-1 < best)
            cand_buf[best][0] = -1;
        if (-1 < *(int*)((char*)g_everquest_guess + off + 0x4C))
            (pinstCEverQuest_guess->petitionCount_guess)++;
        off = off + 0x74;
    } while (off < 0xB54);
    if (EQ_GLOBAL(int*, 0x006E4DF4) != NULL)
    {
        if (EQ_GLOBAL(int*, 0x006E4DF4)[0x23] == 1 &&
            old_count != pinstCEverQuest_guess->petitionCount_guess)
        {
            ((void (__fastcall *)(void*))(*(u32**)EQ_GLOBAL(void*, 0x006E4DF4))[2])(EQ_GLOBAL(void*, 0x006E4DF4));
            ((void (__fastcall *)(void*))(*(u32**)EQ_GLOBAL(void*, 0x006E4DF4))[1])(EQ_GLOBAL(void*, 0x006E4DF4));
            EQ_METHOD(0, void, 0x00436946, (void*), EQ_GLOBAL(void*, 0x006E4DF4))();
        }
        EQ_GLOBAL(int*, 0x006E4DF4)[0x2C] = 0;
    }
    return 1;
}

// FUNCTION: TRILOGY 0x00491dd7
// Applies a server-reported charge use to an item in the player's inventory.
// @early-stop: bag-slot division order and delta register assignment still differ
// ORIGINAL FILE: unknown (subsystem from the call graph)
// __cdecl message helper. With flag -1 and a negative delta, takes -delta (as a byte) charges from the item
// in inventory slot `slot` (0..29, or 30.. = bag (slot-30)/10, position (slot-30)%10); an item with no more
// charges than that is deleted and its slot cleared. Returns 1 when there was an item, else 0.
int UseItemCharges_guess(int slot, int flag, int delta)
{
    EQ_Item** where;
    u8 used;

    if (flag != -1 || delta > -1)
        return 0;
    if (slot < 30)
    {
        where = &g_pc_guess->inventory_items[slot];
    }
    else
    {
        int bagSlot = slot - 30;
        int bagIndex = bagSlot / 10;
        int bagPosition = bagSlot % 10;
        EQ_Item* container = g_pc_guess->inventory_items[bagIndex];
        if (container == NULL)
            return 0;
        if (container->type != EQ_ITEM_CONTAINER)
            return 0;
        where = &((EQ_Container*)container)->contents[bagPosition];
    }
    EQ_Item* item = *where;
    if (item != NULL)
    {
        used = (u8)-delta;
        if (used >= ((EQ_Equipment*)item)->charges)
        {
            operator delete(item);
            item = NULL;
        }
        else
        {
            ((EQ_Equipment*)item)->charges -= used;
        }
        *where = item;
        return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x00491e6e
// Applies server spellbook and gem updates and refreshes local spell timing.
int HandleSpellStateUpdate_guess(int updateType, int entryNumber, int spellNumber)
{
    char message[1024];
    int* pending;
    int* spell;
    int spellId;
    int value;
    char* state;

    if (updateType != -1)
    {
        if (updateType != 0)
        {
            if (updateType != 1)
            {
                if (updateType != 2)
                {
                    if (updateType == 3)
                    {
                        if (g_local_player_guess == NULL)
                            return 1;
                        if ((entryNumber >= 8) && (entryNumber != 9))
                            return 1;
                        if (spellNumber < 0)
                            return 1;
                        if (spellNumber >= 3000)
                            return 1;
                        spellId = EQ_ARRAY(int, 0x005FE9A0)[spellNumber];
                        if (spellId == 0)
                            return 1;
                        value = *(int*)((char*)g_display_guess + 0xBC)
                            + *(int*)((char*)spellId + 0x1B8);
                        *(int*)((char*)g_local_player_guess->actor_info + entryNumber * 4 + 0xD4) = value;
                        if (entryNumber >= 8)
                            return 1;
                        *(char*)((char*)g_local_player_guess->actor_info + 0x24A) = (char)0xFF;
                        if (g_pc_guess->class_ == '\b')
                        {
                            g_local_player_guess->actor_info->unk_0x15C =
                                *(int*)((char*)g_display_guess + 0xBC);
                            return 1;
                        }
                        else
                        {
                            g_local_player_guess->actor_info->unk_0x15C =
                                *(int*)((char*)g_display_guess + 0xBC)
                                + *(int*)((char*)spellId + 0x1B4);
                        }
                    }
                }
                else
                {
                    pending = (int*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x94);
                    if (*pending > 0)
                        --*pending;
                    else
                        *pending = 0;
                    *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xE60 + entryNumber * 4) = 1;
                    pinstEQ_Main_guess->pendingGem_guess = (short)entryNumber;
                    pinstEQ_Main_guess->uiDirty_guess = 1;
                }
            }
            else
            {
                pending = (int*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x94);
                if (*pending > 0)
                    --*pending;
                else
                    *pending = 0;
                pinstEQ_Main_guess->cursorSpell_guess = 0;
                g_pc_guess->spell_memorized[entryNumber] = (short)spellNumber;
                spell = &EQ_ARRAY(int, 0x005FE9A0)[spellNumber];
                sprintf(message, EQ_ARRAY(char, 0x0055E108), (const char*)*spell);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x108);
                *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xE60 + entryNumber * 4) = 4;
                *(int*)((char*)g_local_player_guess->actor_info + entryNumber * 4 + 0xD4) =
                    *(int*)((char*)*spell + 0x1B8) + 1000 + *(int*)((char*)g_display_guess + 0xBC);
                pinstEQ_Main_guess->pendingGem_guess = -1;
                pinstEQ_Main_guess->uiDirty_guess = 1;
            }
        }
        else
        {
            state = (char*)EQ_GLOBAL(void*, 0x006E4E44);
            pending = (int*)(state + 0x94);
            if (*pending > 0)
            {
                *pending = *pending - 1;
            }
            else
            {
                *pending = 0;
            }
            pinstEQ_Main_guess->cursorSpell_guess = 0;
            g_pc_guess->spell_book[entryNumber] = (short)spellNumber;
            sprintf(message, EQ_ARRAY(char, 0x0055E0E8), (const char*)EQ_ARRAY(int, 0x005FE9A0)[spellNumber]);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x108);
            pinstEQ_Main_guess->uiDirty_guess = 1;
        }
    }
    else
    {
        pending = (int*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x94);
        if (*pending > 0)
            --*pending;
        else
            *pending = 0;
    }
    return 1;
}

// Owner class unknown: an interface seen through its vtable, slot 2 (+0x08, no arguments).
class VtblSlot2_guess
{
public:
    virtual void vfunc0();
    virtual void vfunc1();
    virtual void vfunc2();
};

// FUNCTION: TRILOGY 0x00492102
// Applies a server item move: moves, stacks, swaps or destroys an item in character or bag inventory slots.
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
// @early-stop: from_slot stays in esi (read after the calls in the -1 path), so the bag temps, the ebx zero cache and the 0xC spill slot differ.
// __cdecl. Moves the item in inventory slot from_slot into to_slot, or swaps the two. Slots 0..29 are the
// character's own; slot s >= 30 is entry (s - 30) % 10 of the container (type 1) in slot (s - 30) / 10.
// Returns 0 when from_slot is empty, when a bag slot has no container, or when MoveItemQty refuses
// a merge. to_slot -1 destroys the item in
// from_slot and returns 1: the objects at 0x006018C0 and 0x00604984 get a slot-2 vtable call (the second
// one after setting +0x1E8), the objects at 0x006E4E5C and 0x006E4E58 that hold the item drop it, and from
// slot 0 also clears the byte at 0x006E4DE0+0x3C and the word at 0x006E4E44+0xFE8.
// Otherwise a stack of the same item merges through MoveItemQty (count), and anything else swaps.
// The flag at 0x006E4DE0+0x3C is set when inventory slot 0 holds an item.
/* fake match: the swap is written twice (dead ptr == 0 arm and the else-if) to get the original's block order. */
int msg_move_item_slots_guess(int from_slot, int to_slot, int count)
{
    EQ_Item* ptr;
    EQ_Item* local_c;
    EQ_Item* pEVar1;
    EQ_Item** ppEVar3;
    EQ_Item** ppEVar6;
    int* obj;
    char* state;
    int bagSlot;

    ptr = 0;
    local_c = 0;
    if (from_slot < 0x1E)
    {
        ppEVar3 = &g_pc_guess->inventory_items[from_slot];
    }
    else
    {
        bagSlot = from_slot - 0x1E;
        pEVar1 = g_pc_guess->inventory_items[bagSlot / 10];
        if (pEVar1 == 0)
            return 0;
        if (pEVar1->type != EQ_ITEM_CONTAINER)
            return 0;
        ppEVar3 = (EQ_Item**)(pEVar1[1].name + (bagSlot % 10) * 4);
    }
    if (*ppEVar3 == 0)
        return 0;
    if (to_slot == -1)
    {
        obj = EQ_GLOBAL(int*, 0x006018C0);
        if (obj != 0 && obj[4] != 0)
            ((VtblSlot2_guess*)obj)->vfunc2();
        obj = EQ_GLOBAL(int*, 0x00604984);
        if (obj != 0 && obj[4] != 0)
        {
            obj[0x7A] = 1;
            ((VtblSlot2_guess*)EQ_GLOBAL(int*, 0x00604984))->vfunc2();
        }
        obj = EQ_GLOBAL(int*, 0x006E4E5C);
        if (obj[4] != 0 && (EQ_Item*)obj[0x5D] == *ppEVar3)
        {
            ((EQ_Item*)obj[0x5D])[1].lore[7] = 0;
            ((VtblSlot2_guess*)EQ_GLOBAL(int*, 0x006E4E5C))->vfunc2();
            EQ_GLOBAL(int*, 0x006E4E5C)[0x5D] = 0;
        }
        obj = EQ_GLOBAL(int*, 0x006E4E58);
        if (obj[4] != 0 && (EQ_Item*)obj[0x5D] == *ppEVar3)
        {
            ((EQ_Item*)obj[0x5D])[1].lore[7] = 0;
            ((VtblSlot2_guess*)EQ_GLOBAL(int*, 0x006E4E58))->vfunc2();
            EQ_GLOBAL(int*, 0x006E4E58)[0x5D] = 0;
        }
        operator delete(*ppEVar3);
        *ppEVar3 = 0;
        if (from_slot != 0)
            return 1;
        g_cdisplay_guess->itemOnCursor_guess = 0;
        state = (char*)EQ_GLOBAL(void*, 0x006E4E44);
        if (state == 0)
            return 1;
        *(short*)(state + 0xFE8) = 0;
        return 1;
    }
    if (to_slot < 0x1E)
    {
        ppEVar6 = &g_pc_guess->inventory_items[to_slot];
    }
    else
    {
        bagSlot = to_slot - 0x1E;
        pEVar1 = g_pc_guess->inventory_items[bagSlot / 10];
        if (pEVar1 == 0)
            return 0;
        if (pEVar1->type != EQ_ITEM_CONTAINER)
            return 0;
        ppEVar6 = (EQ_Item**)(pEVar1[1].name + (bagSlot % 10) * 4);
    }
    if (ppEVar3 == 0)
        return 0;
    if (ppEVar6 == 0)
        return 0;
    ptr = *ppEVar3;
    local_c = *ppEVar6;
    if (ptr == 0)
    {
        if (local_c == 0)
            return 0;
        pEVar1 = ptr;
        ptr = local_c;
        local_c = pEVar1;
    }
    else if (local_c == 0 || EQ_CDECL(int, 0x004A1D83, (EQ_Item*))(ptr) == 0
        || ptr->item_nr != local_c->item_nr)
    {
        pEVar1 = ptr;
        ptr = local_c;
        local_c = pEVar1;
    }
    else
    {
        if (EQ_CDECL(int, 0x004A1B8C, (EQ_Item**, EQ_Item**, int, int, int))(&ptr, &local_c, count, 0, 0) == 0)
            return 0;
    }
    *ppEVar3 = ptr;
    *ppEVar6 = local_c;
    if (g_pc_guess->inventory_items[0] != 0)
        g_cdisplay_guess->itemOnCursor_guess = 1;
    else
        g_cdisplay_guess->itemOnCursor_guess = 0;
    return 1;
}

// FUNCTION: TRILOGY 0x0049231b
// __cdecl. Builds and displays the missing-component line from six fixed-stride text fields.
void ReportMissingComponents_guess(int unused1, int unused2, const u8* message)
{
    char text[512];
    const char* component;
    int count;
    short componentCount = 0;

    switch (*(const int*)(message + 0xCC))
    {
    case 1:
        strcpy(text, EQ_ARRAY(char, 0x0055E130));
        break;
    case 2:
        strcpy(text, EQ_ARRAY(char, 0x0055E14C));
        break;
    default:
        strcpy(text, EQ_ARRAY(char, 0x0055E16C));
        break;
    }
    component = (const char*)message + 0x0C;
    count = 6;
    do
    {
        if (strlen(component) >= 1)
        {
            if (componentCount > 0)
                strcat(text, EQ_ARRAY(char, 0x0055E12C));
            strcat(text, component);
            ++componentCount;
        }
        component += 0x20;
        --count;
    } while (count != 0);
    strcat(text, EQ_ARRAY(char, 0x00545998));
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(text, 0x108);
}

// FUNCTION: TRILOGY 0x004923c8
// Sends the spell/exe checksums, keeps the character list and dresses the model as the last-chosen character.
void HandleCharacterSelectInfo_guess(void* connection, u32 opcode, MsgCharacterSelect* message, u32 messageSize)
{
    typedef DWORD (WINAPI *ProfileStringReader)(LPCSTR, LPCSTR, LPCSTR, LPSTR, DWORD, LPCSTR);
    typedef BOOL (WINAPI *ProfileStringWriter)(LPCSTR, LPCSTR, LPCSTR, LPCSTR);
    char itemNameBuffer[256];
    char lastCharacterSelection[32];
    int checksumResult;
    int selectedCharacterIndex;
    int materialIndex;
    int modelId;
    short* selectedIndexAddress;
    MsgCharacterSelect* characterSelect;
    EQPlayer* localPlayer;
    const char* itemName;
    short raceMinusOne;
    u32 cameraRace;
    u32 cameraGender;
    u32 cameraClass;
    char isEvil;
    f32 playerHeight;

    checksumResult = EQ_CDECL(int, 0x004A3F60, (void))();
    if ((checksumResult == 0) || ((checksumResult = EQ_CDECL(int, 0x004A4116, (void))()) == 0))
    {
        DbgLog_guess(EQ_ARRAY(char, 0x0055E18C));
        *(u32*)((char*)g_everquest_guess + 0x223F4) = 1;
        *(u32*)((char*)g_everquest_guess + 4) = 0xFF;
        EQ_GLOBAL(u32, 0x006B63F4) = 0xCB;
        *(u32*)g_everquest_guess = 1;
        if (EQ_GLOBAL(void*, 0x006B3194) != 0)
        {
            EQ_CDECL(void, 0x004F21D0, (void*, int, int, int))
                (EQ_GLOBAL(void*, 0x006B3194), 10000, 0, 0);
            EQ_GLOBAL(void*, 0x006B3194) = 0;
        }
        return;
    }

    memcpy((char*)g_everquest_guess + 0x22400, message, 0x494);
    if (EQ_GLOBAL(void*, 0x006E4E6C) == 0)
    {
        EQ_GLOBAL(u32, 0x006B6490) = 0;
        return;
    }

    EQ_GLOBAL(ProfileStringReader, 0x0053418C)
        (EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x0055E180), EQ_ARRAY(char, 0x0053DB48),
         lastCharacterSelection, 0x20, EQ_ARRAY(char, 0x0053DB50));
    if (lastCharacterSelection[0] == '*')
    {
        *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80) = 0;
    }
    else
    {
        selectedCharacterIndex = atoi(lastCharacterSelection);
        selectedIndexAddress = (short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
        *selectedIndexAddress = (short)selectedCharacterIndex;
        if ((*selectedIndexAddress >= 0) && (*selectedIndexAddress < 8))
            goto selected_character_ready;
        *selectedIndexAddress = 0;
    }
    EQ_GLOBAL(ProfileStringWriter, 0x00534188)
        (EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x0055E180), EQ_ARRAY(char, 0x0053D860),
         EQ_ARRAY(char, 0x0053DB50));

selected_character_ready:
    selectedIndexAddress = (short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
    if ((*selectedIndexAddress != -1) &&
        (strcmp(((MsgCharacterSelect*)((char*)g_everquest_guess + 0x22400))->name[*selectedIndexAddress],
             EQ_ARRAY(char, 0x005454F0)) == 0))
    {
        *selectedIndexAddress = -1;
    }
    if (*(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80) >= 0)
    {
        characterSelect = (MsgCharacterSelect*)((char*)g_everquest_guess + 0x22400);
        localPlayer = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
        selectedCharacterIndex = *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
        localPlayer->appearance[5] = characterSelect->appearance[5][selectedCharacterIndex];
        selectedCharacterIndex = *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
        localPlayer->appearance[1] = characterSelect->appearance[1][selectedCharacterIndex];
        selectedCharacterIndex = *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
        localPlayer->appearance[4] = characterSelect->appearance[4][selectedCharacterIndex];
        selectedCharacterIndex = *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
        localPlayer->appearance[0] = characterSelect->appearance[0][selectedCharacterIndex];
        selectedCharacterIndex = *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
        localPlayer->appearance[2] = characterSelect->appearance[2][selectedCharacterIndex];
        selectedCharacterIndex = *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
        localPlayer->appearance[3] = characterSelect->appearance[3][selectedCharacterIndex];

        selectedCharacterIndex = *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
        raceMinusOne = (short)(characterSelect->race[selectedCharacterIndex] - 1);
        cameraRace = ((u32)g_everquest_guess & 0xFFFF0000) | (u16)raceMinusOne;
        cameraGender = (cameraRace & 0xFFFFFF00) | characterSelect->gender[selectedCharacterIndex];
        cameraClass = ((u32)localPlayer & 0xFFFFFF00) | characterSelect->class_[selectedCharacterIndex];
        EQ_METHOD(4, void, 0x00430381, (void*, u32, u32, int, u32),
            EQ_GLOBAL(void*, 0x006E4E6C))
            (cameraGender, cameraRace, 0, cameraClass);

        for (materialIndex = 0; materialIndex < 9; ++materialIndex)
        {
            localPlayer->equipment[materialIndex] = 0;
            localPlayer->equip_colors[materialIndex] = 0;
        }
        for (materialIndex = 0; materialIndex < 9; ++materialIndex)
        {
            selectedCharacterIndex = *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
            if (materialIndex == 0)
            {
                EQ_METHOD(6, void, 0x00407334, (void*, EQPlayer*, int, int, int, u32, int),
                    g_display_guess)
                    (localPlayer, characterSelect->equipment[selectedCharacterIndex][0],
                     0, localPlayer->equipment[0], characterSelect->equip_colors[selectedCharacterIndex][0], 0);
            }
            else
            {
                EQ_METHOD(5, void, 0x0040798E, (void*, EQPlayer*, int, int, int, u32),
                    g_display_guess)
                    (localPlayer, materialIndex, characterSelect->equipment[selectedCharacterIndex][materialIndex],
                     localPlayer->equipment[materialIndex], characterSelect->equip_colors[selectedCharacterIndex][materialIndex]);
            }
        }

        selectedCharacterIndex = *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
        EQ_METHOD(3, void, 0x00406EDC, (void*, EQPlayer*, int, int), g_display_guess)
            (localPlayer, characterSelect->face[selectedCharacterIndex], localPlayer->face);

        selectedCharacterIndex = *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
        modelId = (int)characterSelect->primary_model[selectedCharacterIndex];
        if (modelId <= 0)
        {
            itemName = EQ_ARRAY(char, 0x0053D860);
        }
        else
        {
            sprintf(itemNameBuffer, EQ_ARRAY(char, 0x0053D7D4), modelId);
            itemName = itemNameBuffer;
        }
        EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)
            (localPlayer, 7, itemName);

        selectedCharacterIndex = *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
        modelId = (int)characterSelect->secondary_model[selectedCharacterIndex];
        if (modelId <= 0)
        {
            itemName = EQ_ARRAY(char, 0x0053D860);
        }
        else
        {
            sprintf(itemNameBuffer, EQ_ARRAY(char, 0x0053D7D4), modelId);
            itemName = itemNameBuffer;
        }
        EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)
            (localPlayer, 8, itemName);

        selectedCharacterIndex = *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
        isEvil = EQ_METHOD(3, char, 0x00431509, (void*, int, int, int),
            EQ_GLOBAL(void*, 0x006E4E6C))
            (characterSelect->race[selectedCharacterIndex], characterSelect->class_[selectedCharacterIndex],
             characterSelect->deity[selectedCharacterIndex]);
        localPlayer->y = 0.0f;
        if (isEvil == 0)
        {
            localPlayer->x = 0.0f;
            playerHeight = EQ_GLOBAL(f32, 0x005353B8);
        }
        else
        {
            localPlayer->x = EQ_GLOBAL(f32, 0x005353C0);
            playerHeight = EQ_GLOBAL(f32, 0x005353BC);
        }
        localPlayer->z = playerHeight;
        EQ_METHOD(1, void, 0x00410ABF, (void*, EQPlayer*), g_display_guess)(localPlayer);
    }
    EQ_GLOBAL(u32, 0x006B6490) = 0;
    return;
}

class NetPlayerBuffConstruction_guess : public EQPlayer
{
public:
    // EXTERNAL: TRILOGY 0x004B03EB NetPlayerBuffConstruction_guess::NetPlayerBuffConstruction_guess
    NetPlayerBuffConstruction_guess(EQPlayer* image, int firstFlag, int secondFlag,
        int thirdFlag, int fourthFlag);
};

// FUNCTION: TRILOGY 0x0049286e
// Adds an individual incoming spawn to the zone and initializes its actor.
void msg_eq_netplayerbuff(void* connection, u32 opcode, NetSpawn* spawn, u32 size)
{
    typedef HGLOBAL (WINAPI *GlobalAllocFunction)(UINT, DWORD);
    typedef HGLOBAL (WINAPI *GlobalFreeFunction)(HGLOBAL);
    EQPlayer* unpackedPlayer;
    EQPlayer* player;
    EQPlayer* duplicatePlayer;
    char* groupName;
    int guildIndex;
    int guildId;
    u32 zero;

    zero = 0;
    unpackedPlayer = (EQPlayer*)EQ_GLOBAL(GlobalAllocFunction, 0x005341A0)(0x40, 0x134);
    if ((u32)unpackedPlayer != zero)
    {
        EQ_CDECL(int, 0x004A39E3, (EQPlayer*, NetSpawn*, int))(unpackedPlayer, spawn, 1);
        if ((u32)EQ_GLOBAL(void*, 0x006E4D58) != zero)
        {
            duplicatePlayer = EQ_METHOD(1, EQPlayer*, 0x004B1416, (EQPlayer*, int),
                EQ_GLOBAL(EQPlayer*, 0x006E4D58))(unpackedPlayer->spawn_id);
            if ((u32)duplicatePlayer != zero)
            {
                EQ_GLOBAL(GlobalFreeFunction, 0x00534038)(unpackedPlayer);
                return;
            }
        }

        spawn = (NetSpawn*)new NetPlayerBuffConstruction_guess(unpackedPlayer, 0, 0, 0, 0);
        player = (EQPlayer*)spawn;
        EQ_GLOBAL(GlobalFreeFunction, 0x00534038)(unpackedPlayer);

        if ((u32)player == zero)
        {
            *(u32*)g_everquest_guess = 1;
        }
        else
        {
            if (((u32)player->type == zero) && (player->guild_id != -1))
            {
                guildIndex = EQ_CDECL(int, 0x004DFE8D, (int))(player->guild_id);
                if (guildIndex == -1)
                {
                    spawn = (NetSpawn*)(int)player->guild_id;
                    send_message_guess(g_world_connection_guess, 0x2128, &spawn, 4, 1);
                }
            }

            if (player->class_ == 0x17)
            {
                player->texture = 4;
                EQ_METHOD(1, void, 0x004060A7, (void*, EQPlayer*),
                    g_display_guess)(player);
                if ((u32)player->face != zero)
                {
                    EQ_METHOD(3, void, 0x00406EDC, (void*, EQPlayer*, int, int),
                        g_display_guess)(player, player->face, 0);
                }
            }

            if (12000 < (u32)(((CDisplay*)g_display_guess)->time_ms_guess -
                player->actor_info->lastActiveTime_guess))
            {
                EQ_METHOD(4, void, 0x004AE653, (void*, EQPlayer*, f32, f32, f32),
                    g_display_guess)(player, player->y, player->x, player->z);
            }
            player->actor_info->lastActiveTime_guess =
                ((CDisplay*)g_display_guess)->time_ms_guess;
            EQ_METHOD(1, void, 0x00410ABF, (void*, EQPlayer*),
                g_display_guess)(player);

            if (((u32)player->type == zero) && ((u32)EQ_GLOBAL(char, 0x005FE968) != zero))
            {
                spawn = (NetSpawn*)g_death_state_guess;
                groupName = g_group_names_guess;
                do
                {
                    if (*groupName != 0)
                    {
                        if (strcmp(groupName, player->name) == zero)
                        {
                            *(EQPlayer**)spawn = player;
                        }
                    }
                    spawn = (NetSpawn*)((char*)spawn + 4);
                    groupName += 0x1E;
                } while ((int)groupName < 0x005FE953);
            }

            if ((u32)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8) != zero)
            {
                if (strcmp(EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->name, player->name) == zero)
                {
                    EQ_GLOBAL(EQPlayer*, 0x006E4D8C) = player;
                    player->stand_state = 100;
                    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->stand_state = 100;
                    EQ_GLOBAL(EQPlayer*, 0x006E4DA0) = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
                    EQ_GLOBAL(u32, 0x006E4DA4) = 0;
                    EQ_METHOD(1, void, 0x0040E422, (void*, void*),
                        g_display_guess)((void*)*(u32*)player->actor_info);
                    EQ_GLOBAL(u32, 0x006B3190) = 1;
                }
            }

            if (((u32)player->type == zero) && (player->guild_id != -1))
            {
                guildIndex = EQ_CDECL(int, 0x004DFE8D, (int))(player->guild_id);
                if (guildIndex == -1)
                {
                    guildId = (int)player->guild_id;
                    send_message_guess(g_world_connection_guess, 0x2128, &guildId, 4, 1);
                }
            }
        }
    }
    return;
}

class NetPlayerConstruction_guess : public EQPlayer
{
public:
    // EXTERNAL: TRILOGY 0x004B03EB NetPlayerConstruction_guess::NetPlayerConstruction_guess
    NetPlayerConstruction_guess(EQPlayer* image, int firstFlag, int secondFlag,
        int thirdFlag, int fourthFlag);
};

// FUNCTION: TRILOGY 0x00492ada
// Creates the spawns listed in a received zone batch.
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
void msg_eq_netplayer(void* connection, u32 opcode, void* data, u32 size)
{
    typedef HGLOBAL (WINAPI *GlobalAllocFunction)(UINT, DWORD);
    typedef HGLOBAL (WINAPI *GlobalFreeFunction)(HGLOBAL);
    HGLOBAL playerBuffer;
    EQPlayer* unpackedPlayer;
    EQPlayer* player;
    void* packetData;
    u32 packetSize;
    u32* inflateStream;
    u32 totalOutputBytes;
    int guildId;
    int guildIndex;
    EQPlayer** groupPlayer;
    char* groupName;

    playerBuffer = EQ_GLOBAL(GlobalAllocFunction, 0x005341A0)(0x40, 0x134);
    if (playerBuffer != 0)
    {
        packetSize = size;
        packetData = data;
        EQ_CDECL(void, 0x004A3911, (void*, u32))(packetData, packetSize);
        inflateStream = EQ_ARRAY(u32, 0x005962F8);
        inflateStream[0] = (u32)packetData;
        inflateStream[1] = packetSize;
        inflateStream[2] = 0;
        inflateStream[3] = (u32)EQ_ARRAY(NetSpawn, 0x005DFBC8);
        inflateStream[4] = 0x10680;
        inflateStream[5] = 0;
        inflateStream[11] = 0;
        EQ_CDECL(int, 0x004F84D0, (void*, int, const char*, int))
            (inflateStream, 0x0F, EQ_ARRAY(char, 0x0055DD70), 0x38);
        EQ_CDECL(int, 0x004F8600, (void*, int))(inflateStream, 4);
        totalOutputBytes = inflateStream[5];
        EQ_CDECL(int, 0x004F8480, (void*))(inflateStream);
        *(u32*)&data = totalOutputBytes / 0xA8;
        if ((u32)data != 0)
        {
            size = (u32)EQ_ARRAY(NetSpawn, 0x005DFBC8);
            do
            {
                unpackedPlayer = (EQPlayer*)playerBuffer;
                EQ_CDECL(int, 0x004A39E3, (EQPlayer*, NetSpawn*, int))
                    (unpackedPlayer, (NetSpawn*)size, 0);
                if ((u32)EQ_GLOBAL(void*, 0x006E4D58) != 0 &&
                    EQ_METHOD(1, EQPlayer*, 0x004B1416, (EQPlayer*, int),
                        EQ_GLOBAL(EQPlayer*, 0x006E4D58))(unpackedPlayer->spawn_id) != 0)
                {
                    break;
                }

                player = new NetPlayerConstruction_guess(unpackedPlayer, 0, 0, 0, 0);
                if (player == 0)
                {
                    *(u32*)g_everquest_guess = 1;
                    goto FreePlayerBuffer;
                }

                if (((u32)player->type == 0) && (player->guild_id != -1))
                {
                    guildIndex = EQ_CDECL(int, 0x004DFE8D, (int))(player->guild_id);
                    if (guildIndex == -1)
                    {
                        guildId = (int)player->guild_id;
                        send_message_guess(g_world_connection_guess, 0x2128, &guildId, 4, 1);
                    }
                }

                if (player->class_ == 0x17)
                {
                    player->texture = 4;
                    EQ_METHOD(1, void, 0x004060A7, (void*, EQPlayer*),
                        g_display_guess)(player);
                    if (player->face > 0)
                    {
                        EQ_METHOD(3, void, 0x00406EDC, (void*, EQPlayer*, int, int),
                            g_display_guess)(player, player->face, 0);
                    }
                }

                if (12000 < (u32)(((CDisplay*)g_display_guess)->time_ms_guess -
                    player->actor_info->lastActiveTime_guess))
                {
                    EQ_METHOD(4, void, 0x004AE653, (void*, EQPlayer*, f32, f32, f32),
                        g_display_guess)(player, player->y, player->x, player->z);
                }
                player->actor_info->lastActiveTime_guess =
                    ((CDisplay*)g_display_guess)->time_ms_guess;
                EQ_METHOD(1, void, 0x00410ABF, (void*, EQPlayer*),
                    g_display_guess)(player);

                if (((u32)player->type == 0) &&
                    ((u32)EQ_GLOBAL(char, 0x005FE968) != 0))
                {
                    groupPlayer = EQ_ARRAY(EQPlayer*, 0x005FE954);
                    groupName = g_group_names_guess;
                    do
                    {
                        if (*groupName != 0)
                        {
                            if (strcmp(groupName, player->name) == 0)
                            {
                                *groupPlayer = player;
                            }
                        }
                        groupPlayer++;
                        groupName += 0x1E;
                    } while ((int)groupName < 0x005FE953);
                }

                if ((u32)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8) != 0)
                {
                    if (strcmp(EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->name, player->name) == 0)
                    {
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C) = player;
                        player->stand_state = 100;
                        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->stand_state = 100;
                        EQ_GLOBAL(EQPlayer*, 0x006E4DA0) = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
                        EQ_GLOBAL(u32, 0x006E4DA4) = 0;
                        EQ_METHOD(1, void, 0x0040E422, (void*, void*),
                            g_display_guess)((void*)*(u32*)player->actor_info);
                        EQ_GLOBAL(u32, 0x006B3190) = 1;
                    }
                }

                (*(u32*)&data)--;
                size += 0xA8;
            } while ((u32)data != 0);
        }
FreePlayerBuffer:
        EQ_GLOBAL(GlobalFreeFunction, 0x00534038)(playerBuffer);
    }
    return;
}

// FUNCTION: TRILOGY 0x00492D9B
// Merchant sale reply: credits the price, removes the sold item (bag contents too) or prints the refusal.
// @early-stop: 225 instruction differences remain in frame/spill layout and call setup.
void HandleMerchantSellReply_guess(int unused1, int unused2, int sellMessageOrChildIndex)
{
    char messageBuffer[256];
    EQ_Item* soldItem;
    int saleCount;
    int slotData;
    int saleResult;
    int* object;
    int* progressCounter;
    short inventorySlot;

    if (EQ_GLOBAL(void*, 0x006E4E44) == 0)
        return;

    object = (int*)EQ_GLOBAL(void*, 0x006018C0);
    if (object != 0 && object[4] != 0)
        ((EQMessageTarget_guess*)object)->slot08_guess();

    object = (int*)EQ_GLOBAL(void*, 0x00604984);
    if (object != 0 && object[4] != 0)
    {
        saleCount = 1;
        object[0x7A] = saleCount;
        ((EQMessageTarget_guess*)EQ_GLOBAL(void*, 0x00604984))->slot08_guess();
    }
    else
        saleCount = 1;
    pinstEQ_Main_guess->uiDirty_guess = saleCount;
    slotData = sellMessageOrChildIndex;
    if (sellMessageOrChildIndex == 0 || (inventorySlot = *(short*)(sellMessageOrChildIndex + 8), inventorySlot < 0))
    {
        char* refusalText;
        if (!EQ_GLOBAL(char*, 0x006E4DB8))
            refusalText = (char*)0x0055E1FC;
        else
            refusalText = EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), g_everquest_guess)(
                 EQ_GLOBAL(char*, 0x006E4DB8) + 1);
        sprintf(messageBuffer, (const char*)0x0055E1BC, refusalText);
        refusalText = messageBuffer;
        saleResult = 0x113;
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(refusalText, saleResult);
    }
    else
    {
        if (inventorySlot < 0x1E)
        {
            EQ_Item* pEVar5;
            EQ_Item** pEVar9;
            EQ_Item* saleItem;
            char* merchantName;
            char* format;
            char* amountText;
            int textureSlot;

            pEVar5 = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[inventorySlot];
            if (pEVar5 != 0)
            {
                format = (char*)0x0055E2B4;
                if (EQ_CDECL(int, 0x004A1D83, (EQ_Item*))(pEVar5) != 0)
                {
                    saleItem = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->
                        inventory_items[*(short*)(slotData + 8)];
                    saleCount = ((int)(u8)saleItem[1].lore[0x0B] - 1) /
                        (int)saleItem[1].name[0x0F] + 1;
                    if (!EQ_GLOBAL(char*, 0x006E4DB8))
                        merchantName = (char*)0x0055E2A4;
                    else
                    {
                        merchantName = EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), g_everquest_guess)(
                             EQ_GLOBAL(char*, 0x006E4DB8) + 1);
                        saleItem = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->
                            inventory_items[*(short*)(slotData + 8)];
                    }
                    amountText = EQ_METHOD(2, char*, 0x004385D5, (void*, float, int), saleItem)(
                         pinstEQ_Main_guess->merchantRate_guess, saleCount);
                }
                else
                {
                    saleCount = 1;
                    if (!EQ_GLOBAL(char*, 0x006E4DB8))
                        merchantName = (char*)0x0055E2A4;
                    else
                    {
                        merchantName = EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), g_everquest_guess)(
                             EQ_GLOBAL(char*, 0x006E4DB8) + 1);
                    }
                    amountText = EQ_METHOD(2, char*, 0x004385D5, (void*, float, int), EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->
                         inventory_items[*(short*)(slotData + 8)])(
                         pinstEQ_Main_guess->merchantRate_guess, saleCount);
                    format = (char*)0x0055E280;
                }
                sprintf(messageBuffer, format, amountText, merchantName,
                     EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[*(short*)(slotData + 8)]);
                saleResult = EQ_METHOD(2, int, 0x004385A7, (void*, float, int), EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->
                     inventory_items[*(short*)(slotData + 8)])(
                     pinstEQ_Main_guess->merchantRate_guess, saleCount);
                EQ_METHOD(1, void, 0x00423E16, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))(saleResult);
                if (g_sound_manager_ptr_guess != 0)
                    EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), g_sound_manager_ptr_guess)(0x8A, 0);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(messageBuffer, 0x114);

                soldItem = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->
                    inventory_items[*(short*)(slotData + 8)];
                if (soldItem->type == 1)
                {
                    sellMessageOrChildIndex = 0;
                    if ((u8)soldItem[1].lore[6] > 0)
                    {
                        pEVar9 = (EQ_Item**)(soldItem + 1);
                        do
                        {
                            if (*pEVar9 != 0)
                            {
                                if (EQ_CDECL(int, 0x004A1D83, (EQ_Item*))(*pEVar9) != 0)
                                {
                                    saleCount = ((int)(u8)(*pEVar9)[1].lore[0x0B] - 1) /
                                        (int)(*pEVar9)[1].name[0x0F] + 1;
                                    if (!EQ_GLOBAL(char*, 0x006E4DB8))
                                        merchantName = (char*)0x0055E2A4;
                                    else
                                    {
                                        merchantName = EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), g_everquest_guess)(
                                             EQ_GLOBAL(char*, 0x006E4DB8) + 1);
                                    }
                                    amountText = EQ_METHOD(2, char*, 0x004385D5, (void*, float, int), *pEVar9)(
                                         pinstEQ_Main_guess->merchantRate_guess, saleCount);
                                    format = (char*)0x0055E2B4;
                                }
                                else
                                {
                                    saleCount = 1;
                                    if (!EQ_GLOBAL(char*, 0x006E4DB8))
                                        merchantName = (char*)0x0055E2A4;
                                    else
                                    {
                                        merchantName = EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), g_everquest_guess)(
                                             EQ_GLOBAL(char*, 0x006E4DB8) + 1);
                                    }
                                    amountText = EQ_METHOD(2, char*, 0x004385D5, (void*, float, int), *pEVar9)(
                                         pinstEQ_Main_guess->merchantRate_guess, saleCount);
                                    format = (char*)0x0055E280;
                                }
                                sprintf(messageBuffer, format, amountText, merchantName, soldItem);
                                saleResult = EQ_METHOD(2, int, 0x004385A7, (void*, float, int), *pEVar9)(
                                     pinstEQ_Main_guess->merchantRate_guess, saleCount);
                                EQ_METHOD(1, void, 0x00423E16, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))(saleResult);
                                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(messageBuffer, 0x114);
                            }
                            ++sellMessageOrChildIndex;
                            ++pEVar9;
                        } while (sellMessageOrChildIndex < (int)(u8)soldItem[1].lore[6]);
                    }
                }
                operator delete(EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[*(short*)(slotData + 8)]);
                inventorySlot = *(short*)(slotData + 8);
                EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[inventorySlot] = 0;
                textureSlot = EQ_CDECL(int, 0x004A1DCE, (int, char*))
                    (inventorySlot, (char*)0x0053D860);
                EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, char*), g_display_guess)(
                     EQ_GLOBAL(EQPlayer*, 0x006E4D8C), textureSlot, (char*)0x0053D860);
                EQ_CDECL(void, 0x004A204D, (void*))(EQ_GLOBAL(void*, 0x006E4DA8));
                saleCount = 1;
            }
            else
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(
                     (const char*)0x0055E24C, 0x0D);
            }
        }
        else
        {
            EQ_Item* pEVar5;
            char* merchantName;
            char* saleFormat;
            char* childSlotAddress;
            char* format;
            char* amountText;

            slotData = (int)inventorySlot - 0x1E;
            pEVar5 = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[(short)(slotData / 10)];
            if (pEVar5 == 0 || pEVar5->type != 1)
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(
                     (const char*)0x0055E20C, 0x0D);
            }
            else
            {
                slotData = (int)(short)(slotData % 10);
                childSlotAddress = pEVar5[1].name + slotData * 4;
                if (*(int*)childSlotAddress != 0)
                {
                    if (EQ_CDECL(int, 0x004A1D83, (EQ_Item*))(*(EQ_Item**)childSlotAddress) != 0)
                    {
                        saleCount = ((int)(u8)(*(EQ_Item**)childSlotAddress)[1].lore[0x0B] - 1) /
                            (int)(*(EQ_Item**)childSlotAddress)[1].name[0x0F] + 1;
                        if (!EQ_GLOBAL(char*, 0x006E4DB8))
                            merchantName = (char*)0x0055E2A4;
                        else
                            merchantName = EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), g_everquest_guess)(
                                 EQ_GLOBAL(char*, 0x006E4DB8) + 1);
                        saleFormat = (char*)0x0055E2B4;
                    }
                    else
                    {
                        saleCount = 1;
                        if (!EQ_GLOBAL(char*, 0x006E4DB8))
                            merchantName = (char*)0x0055E2A4;
                        else
                            merchantName = EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), g_everquest_guess)(
                                 EQ_GLOBAL(char*, 0x006E4DB8) + 1);
                        saleFormat = (char*)0x0055E280;
                    }
                    amountText = EQ_METHOD(2, char*, 0x004385D5, (void*, float, int), *(EQ_Item**)childSlotAddress)(
                         pinstEQ_Main_guess->merchantRate_guess, saleCount);
                    sprintf(messageBuffer, saleFormat, amountText, merchantName, *(EQ_Item**)childSlotAddress);
                    saleResult = EQ_METHOD(2, int, 0x004385A7, (void*, float, int), *(EQ_Item**)childSlotAddress)(
                         pinstEQ_Main_guess->merchantRate_guess, saleCount);
                    EQ_METHOD(1, void, 0x00423E16, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))(saleResult);
                    if (g_sound_manager_ptr_guess != 0)
                        EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), g_sound_manager_ptr_guess)(0x8A, 0);
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(messageBuffer, 0x114);
                    operator delete(*(EQ_Item**)childSlotAddress);
                    *(int*)childSlotAddress = 0;
                    saleCount = 1;
                }
                else
                {
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(
                         (const char*)0x0055E24C, 0x0D);
                }
            }
        }
    }

    pinstEQ_Main_guess->merchantReady_guess = saleCount;
    progressCounter = (int*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x94);
    if (*progressCounter > 0)
        *progressCounter = *progressCounter - 1;
    else
        *progressCounter = 0;
    ++EQ_GLOBAL(int, 0x006B6588);
    if (9 < EQ_GLOBAL(int, 0x006B6588))
        EQ_GLOBAL(int, 0x006B6588) = 0;
    EQ_ARRAY(int, 0x006B6560)[EQ_GLOBAL(int, 0x006B6588)] = 0x0C;
    saleResult = (int)time(0);
    EQ_ARRAY(int, 0x006B6538)[EQ_GLOBAL(int, 0x006B6588)] = saleResult;
    EQ_CDECL(void, 0x004A204D, (void*))(EQ_GLOBAL(void*, 0x006E4DA8));
    if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0] != 0)
        g_cdisplay_guess->itemOnCursor_guess = 1;
    else
        g_cdisplay_guess->itemOnCursor_guess = 0;
    pinstEQ_Main_guess->uiDirty_guess = saleCount;
    return;
}

// FUNCTION: TRILOGY 0x0049341c
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
// Resets vehicle-controlled players to the zone safe point and repositions their renderer actors.
void msg_vehicle_reset(void)
{
    float floorZ;

    if (g_local_player_guess->actor_info->ride_target_guess != 0)
    {
        g_local_player_guess->y = EQ_GLOBAL(float, 0x006B6734);
        g_local_player_guess->x = EQ_GLOBAL(float, 0x006B6738);
        g_local_player_guess->z = EQ_GLOBAL(float, 0x006B673C);
        floorZ = CDisplay_GetPlayerFloorHeight_guess(g_display_guess,
            g_local_player_guess, EQ_GLOBAL(float, 0x006B6734),
            EQ_GLOBAL(float, 0x006B6738),
            EQ_GLOBAL(float, 0x006B673C) + 1.0f, 1);
        if (floorZ != EQ_GLOBAL(const float, 0x005351F4))
        {
            g_local_player_guess->actor_info->floor_z = floorZ;
            g_local_player_guess->z = floorZ + g_local_player_guess->height_offset;
        }
        CDisplay_UpdatePlayerActor_guess(g_display_guess, g_local_player_guess);
    }

    if (g_control_player_guess != 0 && g_control_player_guess != g_local_player_guess &&
        g_control_player_guess->actor_info->ride_target_guess != 0)
    {
        g_control_player_guess->y = EQ_GLOBAL(float, 0x006B6734);
        g_control_player_guess->x = EQ_GLOBAL(float, 0x006B6738);
        g_control_player_guess->z = EQ_GLOBAL(float, 0x006B673C);
        floorZ = CDisplay_GetPlayerFloorHeight_guess(g_display_guess,
            g_control_player_guess, EQ_GLOBAL(float, 0x006B6734),
            EQ_GLOBAL(float, 0x006B6738),
            EQ_GLOBAL(float, 0x006B673C) + 1.0f, 1);
        if (floorZ != EQ_GLOBAL(const float, 0x005351F4))
        {
            g_control_player_guess->actor_info->floor_z = floorZ;
            g_control_player_guess->z = floorZ + g_control_player_guess->height_offset;
        }
        CDisplay_UpdatePlayerActor_guess(g_display_guess, g_control_player_guess);
    }
}

// FUNCTION: TRILOGY 0x00493581
// Refreshes the trainer's list of skills and languages the character can train.
// @early-stop: ESI/EBX roles and loop temporaries remain swapped; macro thiscalls add dummy-EDX zeroing.
/* fake match: volatile msg keeps the skill count in its original parameter stack slot. */
/* fake match: step reuses the original zero, mode value, row stride, and language index. */
void __cdecl UpdateTrainerCaps_guess(void* connection, u32 opcode, MsgClassTrain* volatile msg, u32 size)
{
    typedef void (__fastcall *EQTrainingMethod_guess)(void*);
    u8 response[0x5C];
    int* counter;
    int value;
    int step = 0;
    int i;
    int n;
    int listOffset;
    void* client;
    void* soundManager;
    u32 vtable;
    EQTrainingMethod_guess updateTrainingWindow;

    if (EQ_GLOBAL(int, 0x006E4DC4) == step && EQ_GLOBAL(int*, 0x006E4E44) != 0)
    {
        MsgClassTrain* pMsg = msg;
        EQPlayer* trainer = EQ_METHOD(1, EQPlayer*, 0x004B1416, (EQPlayer*, u32), EQ_GLOBAL(EQPlayer*, 0x006E4D58))(pMsg->npc_id);
        if (trainer != 0)
        {
            if (pMsg->open == step)
            {
                if (EQ_GLOBAL(int*, 0x006E4E44) != 0)
                {
                    value = EQ_GLOBAL(int*, 0x006E4E44)[0x397];
                    counter = &EQ_GLOBAL(int*, 0x006E4E44)[0x397];
                    if (value > step)
                        *counter = value - 1;
                    else
                        *counter = step;
                }
                counter = &EQ_GLOBAL(int*, 0x006E4E44)[0x397];
                if (*counter != step)
                {
                    *(int*)(response + 0) = -4;
                    *(int*)(response + 4) = *counter;
                    *(int*)(response + 8) = 9;
                    send_message_guess(g_world_connection_guess, 0x2147, response, 0x5C, 1);
                }
                EQ_GLOBAL(int*, 0x006E4E44)[0x397] = step;
            }
            else
            {
                if (EQ_GLOBAL(int, 0x006EFE1C) == step)
                {
                    soundManager = EQ_GLOBAL(void*, 0x006ECDF4);
                    if (soundManager != 0)
                    {
                        EQ_METHOD(1, void, 0x004E4EE6, (void*, int), soundManager)(0x14);
                    }
                }
                step = 2;
                if (g_game_state_guess == step)
                {
                    client = EQ_GLOBAL(void*, 0x006E4E44);
                    EQ_GLOBAL(int, 0x00590170) = 2;
                    g_game_state_guess = 1;
                    if (client != 0)
                        *(int*)((u8*)client + 0xEB8) = 1;
                    EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
                }
                client = EQ_GLOBAL(void*, 0x006E4E44);
                EQ_GLOBAL(void*, 0x006E4DC4) = trainer;
                *(float*)((u8*)client + 0x1018) = *(float*)((u8*)pMsg + 0x6C);
                client = EQ_GLOBAL(void*, 0x006E4E44);
                *((u8*)client + 0xFE1) = *((u8*)client + 0xFE0);
                client = EQ_GLOBAL(void*, 0x006E4E44);
                vtable = *(u32*)client;
                updateTrainingWindow = (EQTrainingMethod_guess)*(u32*)(vtable + 8);
                updateTrainingWindow(client);
                client = EQ_GLOBAL(void*, 0x006E4E44);
                *((u8*)client + 0xFE0) = 6;
                listOffset = 0x6C7EE;
                n = 0;
                i = listOffset;
                do
                {
                    *(short*)((u8*)EQ_GLOBAL(void*, 0x006E4E44) + i) = -1;
                    i += step;
                    *((u8*)EQ_GLOBAL(void*, 0x006E4E44) + 0x5F038 + n) = 0;
                    n++;
                } while (i < 0x6C8B6);
                i = 0;
                do
                {
                    *((u8*)EQ_GLOBAL(void*, 0x006E4E44) + 0x5F09C + i) = 0;
                    i++;
                } while (i < 0x20);

                msg = 0;
                i = 0;
                do
                {
                    if (i != 0x48)
                    {
                        if (pMsg->skill_max[i] <= 0xFC && pMsg->skill_max[i] > 0 &&
                            EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->skills[i] != 0xFF &&
                            i != 0x21 && i != 0x0F)
                        {
                            (*(int*)&msg)++;
                            *(short*)((u8*)EQ_GLOBAL(void*, 0x006E4E44) + listOffset) = (short)i;
                            listOffset += 2;
                        }
                        *((u8*)EQ_GLOBAL(void*, 0x006E4E44) + 0x5F038 + i) = pMsg->skill_max[i];
                    }
                    i++;
                } while (i < 0x4A);

                step = 0;
                listOffset = 0x6C7EE + *(int*)&msg * 2;
                do
                {
                    if (EQ_CDECL(int, 0x004A08D7, (CharacterProfile*, int))
                        (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8), step) == 0)
                    {
                        pMsg->language_max[step] = 0;
                    }
                    if (pMsg->language_max[step] > 0)
                    {
                        (*(int*)&msg)++;
                        *(short*)((u8*)EQ_GLOBAL(void*, 0x006E4E44) + listOffset) = (short)(step + 100);
                        listOffset += 2;
                    }
                    *((u8*)EQ_GLOBAL(void*, 0x006E4E44) + 0x5F09C + step) = pMsg->language_max[step];
                    step++;
                } while (step < 0x18);
                EQ_GLOBAL(int*, 0x006E4E44)[0x1A170] = *(int*)&msg;
                client = EQ_GLOBAL(void*, 0x006E4E44);
                vtable = *(u32*)client;
                updateTrainingWindow = (EQTrainingMethod_guess)*(u32*)(vtable + 4);
                updateTrainingWindow(client);
            }
        }
    }
    return;
}

class ReceivedEquipmentAllocation_guess : public EQ_Equipment
{
public:
    // EXTERNAL: TRILOGY 0x00436099 ReceivedEquipmentAllocation_guess::ReceivedEquipmentAllocation_guess
    ReceivedEquipmentAllocation_guess();
};

class ReceivedContainerAllocation_guess
{
public:
    u8 bytes[0xDC];
    // EXTERNAL: TRILOGY 0x00434040 ReceivedContainerAllocation_guess::ReceivedContainerAllocation_guess
    ReceivedContainerAllocation_guess();
};

class ReceivedNoteAllocation_guess : public EQ_Note
{
public:
    // EXTERNAL: TRILOGY 0x0044FF75 ReceivedNoteAllocation_guess::ReceivedNoteAllocation_guess
    ReceivedNoteAllocation_guess();
};

// FUNCTION: TRILOGY 0x004937c3
// Stores a received equipment/container/note by slot in a table of the 0x006E4E44 object; drops bogus items.
// @early-stop: EH state resets, register scheduling, and fastcall adapters still differ.
void __cdecl msg_receive_item_guess(void* connection, u32 opcode, void* msg, u32 size)
{
    char itemType;
    void* packet;
    void* item;
    void* client;
    short itemId;
    short imageId;
    short slot;
    int i;
    int deleteItem;
    int* contents;
    char message[0x100];

    client = EQ_GLOBAL(void* volatile, 0x006E4E44);
    item = 0;
    if (client == 0 || *(int*)((u8*)client + 0x685E0) == 0)
        return;

    packet = msg;
    itemType = *((char*)packet + 4);
    deleteItem = 0;
    switch (itemType)
    {
    case 0:
    {
        msg = new ReceivedEquipmentAllocation_guess;
        item = msg;
        memcpy(item, (u8*)packet + 5, 0x124);
        itemId = *(short*)((u8*)item + 0x82);
        if (itemId != 0 && *((u8*)item + 0xC2) != 0x0D)
        {
            imageId = *(short*)((u8*)item + 0x84);
            if (EQ_ARRAY(int, 0x006B32B4)[imageId] == 0)
            {
                sprintf(message, EQ_ARRAY(char, 0x0055E2DC), itemId);
                EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), (CEverQuest*)EQ_GLOBAL(void* volatile, 0x006EFE2C))(
                        message, 0x0D);
                *(short*)((u8*)item + 0x84) = 500;
            }
            slot = *(short*)((u8*)item + 0x86);
            *(int*)((u8*)EQ_GLOBAL(void* volatile, 0x006E4E44) + 0x6C5E0 + slot * 4) = (int)item;
        }
        else
        {
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), (CEverQuest*)EQ_GLOBAL(void* volatile, 0x006EFE2C))(
                    EQ_ARRAY(char, 0x0055E314), 0x0D);
            deleteItem = 1;
        }
        break;
    }
    case 1:
    {
        msg = new ReceivedContainerAllocation_guess;
        item = msg;
        memcpy(item, (u8*)packet + 5, 0xDC);
        if (*(short*)((u8*)item + 0x82) == 0)
        {
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), (CEverQuest*)EQ_GLOBAL(void* volatile, 0x006EFE2C))(
                    EQ_ARRAY(char, 0x0055E314), 0x0D);
            if (item != 0)
            {
                EQ_METHOD(0, void, 0x0043405A, (void*), item)();
                deleteItem = 1;
            }
        }
        else
        {
            *((u8*)item + 0xD6) = 0;
            contents = (int*)((u8*)item + 0xAC);
            for (i = 10; i != 0; --i)
            {
                *contents = 0;
                ++contents;
            }
            imageId = *(short*)((u8*)item + 0x84);
            if (EQ_ARRAY(int, 0x006B32B4)[imageId] == 0)
            {
                itemId = *(short*)((u8*)item + 0x82);
                sprintf(message, EQ_ARRAY(char, 0x0055E2F8), itemId);
                EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), (CEverQuest*)EQ_GLOBAL(void* volatile, 0x006EFE2C))(
                        message, 0x0D);
                *(short*)((u8*)item + 0x84) = 500;
            }
            slot = *(short*)((u8*)item + 0x86);
            *(int*)((u8*)EQ_GLOBAL(void* volatile, 0x006E4E44) + 0x6C5E0 + slot * 4) = (int)item;
        }
        break;
    }
    case 2:
    {
        msg = new ReceivedNoteAllocation_guess;
        item = msg;
        memcpy(item, (u8*)packet + 5, 0xD0);
        itemId = *(short*)((u8*)item + 0x82);
        if (itemId == 0)
        {
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), (CEverQuest*)EQ_GLOBAL(void* volatile, 0x006EFE2C))(
                    EQ_ARRAY(char, 0x0055E314), 0x0D);
            if (item != 0)
            {
                EQ_METHOD(0, void, 0x0040101D, (void*), item)();
                deleteItem = 1;
            }
        }
        else
        {
            imageId = *(short*)((u8*)item + 0x84);
            if (EQ_ARRAY(int, 0x006B32B4)[imageId] == 0)
            {
                sprintf(message, EQ_ARRAY(char, 0x0055E2DC), itemId);
                EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), (CEverQuest*)EQ_GLOBAL(void* volatile, 0x006EFE2C))(
                        message, 0x0D);
                *(short*)((u8*)item + 0x84) = 500;
            }
            slot = *(short*)((u8*)item + 0x86);
            *(int*)((u8*)EQ_GLOBAL(void* volatile, 0x006E4E44) + 0x6C5E0 + slot * 4) = (int)item;
        }
        break;
    }
    }

    if (deleteItem != 0)
        operator delete(item);
    client = EQ_GLOBAL(void* volatile, 0x006E4E44);
    *(int*)((u8*)client + 0xEB8) = 1;
}

// OP_SpawnAppearance (0x20F5): copies the 0xC byte message, looks the spawn up by id and applies the appearance
// type (msg.type) with msg.parameter to it, then refreshes the actor. Unknown spawns and types are ignored.
// FUNCTION: TRILOGY 0x004939EB
// Applies a spawn appearance change (invis, pvp, light, guild/rank with chat notices, anon, GM, levitation).
void __cdecl spawn_appearance_msg_guess(void* connection, u32 opcode, MsgSpawnAppearance* msgIn, u32 size)
{
#define SA_DISPLAY EQ_GLOBAL(void*, 0x006E4DE0)
#define SA_CHAT(text, color) \
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((text), (color))
    char message[256];
    MsgSpawnAppearance msg;
    EQPlayer* player;
    u32 guildIndex;

    memcpy(&msg, msgIn, 0xC);
    player = EQ_METHOD(1, EQPlayer*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))(msg.spawn_id);
    if (player == 0)
        return;
    switch (msg.type)
    {
    case 4:
        player->pvp = (u8)msg.parameter;
        if (player == g_local_player_guess)
            g_pc_guess->pvp = (u8)msg.parameter;
        EQ_METHOD(1, void, 0x00411EE6, (void*, EQPlayer*), SA_DISPLAY)(player);
        break;
    case 3:
        if (player == EQ_GLOBAL(EQPlayer*, 0x006E4DAC) && player != g_local_player_guess
            && msg.parameter != 0 && player->invis == 0 && g_pc_guess != 0
            && EQ_METHOD(1, short, 0x004238A2, (void*, int), g_pc_guess)(0xD) < 1)
            EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = 0;
        player->invis = (u8)msg.parameter;
        EQ_METHOD(2, void, 0x00410D07, (void*, EQPlayer*, u32), SA_DISPLAY)(player, msg.parameter);
        break;
    case 2:
        player->max_hp = (short)msg.parameter;
        break;
    case 1:
        player->level = (u8)msg.parameter;
        break;
    case 0:
        player->type = 2;
        break;
    case 5:
        if (player->light != (u8)msg.parameter)
        {
            player->light = (u8)msg.parameter;
            EQ_METHOD(1, void, 0x00410602, (void*, EQPlayer*), SA_DISPLAY)(player);
        }
        break;
    case 0x11:
        player->cur_hp = (short)msg.parameter;
        break;
    case 0x10:
        player->spawn_id = msg.parameter;
        EQ_METHOD(0, void, 0x004CF829, (void*), g_everquest_guess)();
        break;
    case 0xF:
        player->sneaking = (u8)msg.parameter;
        break;
    case 0xE:
        EQ_METHOD(1, void, 0x004B45CB, (void*, u32), player)(msg.parameter);
        break;
    case 0x12:
        player->linkdead = (u8)msg.parameter;
        EQ_METHOD(1, void, 0x00411B7B, (void*, EQPlayer*), SA_DISPLAY)(player);
        if (player == g_local_player_guess && player->linkdead == 0 && g_pc_guess != 0)
            EQ_METHOD(1, void, 0x0042798F, (void*, int), g_pc_guess)(1);
        break;
    case 0x1D:
        EQ_METHOD(1, void, 0x004B4836, (void*, float), player)((float)(int)msg.parameter);
        break;
    case 0x1B:
        player->unk_0x108 = msg.parameter;
        break;
    case 0x18:
        EQ_METHOD(1, void, 0x004B2548, (void*, u32), player)(msg.parameter);
        break;
    case 0x16:
        if (g_local_player_guess != 0 && player == g_local_player_guess)
        {
            if (g_pc_guess != 0 && (guildIndex = g_pc_guess->guild_id) != msg.parameter)
            {
                if ((int)msg.parameter >= 0)
                {
                    sprintf(message, EQ_ARRAY(char, 0x0055E4B4), EQ_ARRAY(char, 0x006E5494) + msg.parameter * 0x3C);
                    SA_CHAT(message, 0x111);
                }
                else if (EQ_ARRAY(char, 0x006E5494)[guildIndex * 0x3C] == 0)
                    SA_CHAT(EQ_ARRAY(char, 0x0055E48C), 0x111);
                else
                {
                    sprintf(message, EQ_ARRAY(char, 0x0055E468), EQ_ARRAY(char, 0x006E5494) + guildIndex * 0x3C);
                    SA_CHAT(message, 0x111);
                }
                g_pc_guess->guild_id = (short)msg.parameter;
            }
        }
        else if (g_pc_guess != 0 && player->guild_id != (int)msg.parameter)
        {
            if (g_pc_guess->guild_id == (int)msg.parameter && (int)msg.parameter >= 0)
            {
                sprintf(message, EQ_ARRAY(char, 0x0055E44C), player->name);
                SA_CHAT(message, 0x111);
            }
            else if (g_pc_guess->guild_id == player->guild_id && player->guild_id >= 0)
            {
                sprintf(message, EQ_ARRAY(char, 0x0055E424), player->name);
                SA_CHAT(message, 0x111);
            }
        }
        player->guild_id = (short)msg.parameter;
        EQ_METHOD(1, void, 0x00411B7B, (void*, EQPlayer*), SA_DISPLAY)(player);
        break;
    case 0x17:
        if (g_local_player_guess == 0 || player != g_local_player_guess)
        {
            if (g_pc_guess != 0 && (short)player->guild_rank != (int)msg.parameter
                && g_pc_guess->guild_id == player->guild_id && player->guild_id >= 0
                && (int)msg.parameter >= 0 && (int)msg.parameter <= 2)
            {
                switch (msg.parameter)
                {
                case 0:
                    sprintf(message, EQ_ARRAY(char, 0x0055E334), player->name);
                    SA_CHAT(message, 0x111);
                    break;
                case 1:
                    sprintf(message, EQ_ARRAY(char, 0x0055E360), player->name);
                    SA_CHAT(message, 0x111);
                    break;
                case 2:
                    sprintf(message, EQ_ARRAY(char, 0x0055E384), player->name);
                    SA_CHAT(message, 0x111);
                    break;
                }
            }
        }
        else if (g_pc_guess != 0 && (u8)g_pc_guess->guild_rank != msg.parameter
                 && (int)msg.parameter >= 0 && (int)msg.parameter <= 2)
        {
            switch (msg.parameter)
            {
            case 0:
                SA_CHAT(EQ_ARRAY(char, 0x0055E3A8), 0x111);
                break;
            case 1:
                SA_CHAT(EQ_ARRAY(char, 0x0055E3D4), 0x111);
                break;
            case 2:
                SA_CHAT(EQ_ARRAY(char, 0x0055E3FC), 0x111);
                break;
            }
            g_pc_guess->guild_rank = (u8)msg.parameter;
        }
        player->guild_rank = (u8)msg.parameter;
        EQ_METHOD(1, void, 0x00411B7B, (void*, EQPlayer*), SA_DISPLAY)(player);
        break;
    case 0x1E:
        if (player == g_local_player_guess)
            break;
        player->type = 1;
        EQ_METHOD(1, void, 0x00411B7B, (void*, EQPlayer*), SA_DISPLAY)(player);
        break;
    case 0x19:
        player->pet_owner_id = msg.parameter;
        break;
    case 0x14:
        player->gm = (u8)msg.parameter;
        EQ_METHOD(1, void, 0x00411EE6, (void*, EQPlayer*), SA_DISPLAY)(player);
        EQ_METHOD(1, void, 0x00411B7B, (void*, EQPlayer*), SA_DISPLAY)(player);
        if (player == g_local_player_guess && g_pc_guess != 0)
        {
            g_pc_guess->gm_flag = g_local_player_guess->gm;
            EQ_GLOBAL(int, 0x006B7120) = 0x16;
            EQ_METHOD(2, void, 0x004CE8C0, (void*, int, int), g_everquest_guess)(0, 1);
        }
        break;
    case 0x15:
        player->anon = (u8)msg.parameter;
        EQ_METHOD(1, void, 0x00411EE6, (void*, EQPlayer*), SA_DISPLAY)(player);
        EQ_METHOD(1, void, 0x00411B7B, (void*, EQPlayer*), SA_DISPLAY)(player);
        if (player == g_local_player_guess && g_pc_guess != 0)
        {
            g_pc_guess->anon = (u8)g_local_player_guess->anon;
            EQ_GLOBAL(int, 0x006B7120) = 5;
            EQ_METHOD(2, void, 0x004CE8C0, (void*, int, int), g_everquest_guess)(0, 1);
            EQ_GLOBAL(int, 0x006B6860) = g_local_player_guess->anon;
            EQ_METHOD(0, void, 0x004CF31A, (void*), g_everquest_guess)();
        }
        break;
    case 0x13:
        player->levitate = (short)msg.parameter;
        if ((short)msg.parameter == 0)
            *((u8*)player->actor_info + 0x88) = 1;
        else
            *((u8*)player->actor_info + 0x88) = 0;
        if (player->levitate == 2)
            player->actor_info->unknown0x078 = 1;
        else
            player->actor_info->unknown0x078 = -1;
        break;
    }
    EQ_METHOD(1, void, 0x00410ABF, (void*, EQPlayer*), SA_DISPLAY)(player);
#undef SA_DISPLAY
#undef SA_CHAT
}

// Disarm message. msg[9] == -1: the first weapon slot that holds an item (13, 14, then 11) is at risk. Its
// skill (0 for a container or note) and msg[8] give a chance of msg[8] - skill + 10, clamped to 5..20; a
// random 1..100 at or below it moves the weapon to a free slot and sets msg[9] = 1. 0x20AA then reports
// msg[9]. msg[9] > 0 names the target in chat and improves the disarm skill; any other value is a failed
// attempt. The third argument's stack slot is reused to hold the weapon slot (13, 14 or 11).
// FUNCTION: TRILOGY 0x00493fe8
// Disarm: rolls an incoming attempt (weapon unequipped, result sent back) or reports the outcome of our own.
// @early-stop: frame is 0x144 and the bag-slot loop is strength-reduced versus the original.
void __cdecl disarm_msg_guess(int conn, int msgOpcode, MsgBeg* weaponSlot)
{
    char message[256];
    char requestBuf[32];
    int requestSlot;
    struct DisarmReply_guess
    {
        u32 field_0;
        u32 field_4;
        u8 unknown_8;
        u8 result;
        u8 unknown_a[2];
    };
    EQ_Item** invEntry;
    int temp;
    int slotIndex;
    MsgBeg* disarmPacket = weaponSlot;
    char result;
    u16 ownSkill;
    EQ_Item* item;
    EQ_Item* pEVar1;
    EQ_Item* pEVar8;
    EQPlayer* player;
    char* targetName;
    u8 skillId;
    int bagIndex;
    short roll;
    short tmp;
    int* obj;

    result = disarmPacket->success;
    ownSkill = 0;
    temp = 0;
    if (result == -1)
    {
      disarmPacket->success = 0;
    if (g_pc_guess->inventory_items[0xd] != 0)
    {
            weaponSlot = (MsgBeg*)0xd;
        item = g_pc_guess->inventory_items[0xd];
    }
    else if (g_pc_guess->inventory_items[0xe] != 0)
    {
        weaponSlot = (MsgBeg*)0xe;
        item = g_pc_guess->inventory_items[0xe];
    }
    else
    {
        item = g_pc_guess->inventory_items[0xb];
        weaponSlot = (MsgBeg*)0xb;
    }
    if (item == 0)
    {
        disarmPacket->success = 0;
    }
    else
    {
        if (item->type == EQ_ITEM_EQUIPMENT)
        {
            skillId = EQ_METHOD(1, u8, 0x004B422A, (EQPlayer*, u8), g_local_player_guess)(*((u8*)item + 0xC2));
            ownSkill = (u16)g_pc_guess->skills[skillId];
        }
        roll = random_range_guess(1, 100);
        tmp = (disarmPacket->skill - ownSkill) + 10;
        if (tmp < 5)
            tmp = 5;
        if (0x14 < tmp)
            tmp = 0x14;
        if (roll <= tmp)
        {
            disarmPacket->success = 1;
            obj = EQ_GLOBAL(int*, 0x006018C0);
            if (obj != 0 && obj[4] != 0)
                ((VtblSlot2_guess*)obj)->vfunc2();
            obj = EQ_GLOBAL(int*, 0x00604984);
            if (obj != 0 && obj[4] != 0)
            {
                obj[0x7A] = 1;
                ((VtblSlot2_guess*)EQ_GLOBAL(int*, 0x00604984))->vfunc2();
            }
            slotIndex = 0x16;
            invEntry = &g_pc_guess->inventory_items[0x16];
            do
            {
                pEVar1 = *invEntry;
                if (pEVar1 == 0)
                {
                    temp = slotIndex;
checkFound:
                    if ((short)temp != 0)
                        break;
                }
                else if (pEVar1->type == EQ_ITEM_CONTAINER && (bagIndex = 0, pEVar1[1].lore[6] != 0))
                {
                    pEVar8 = pEVar1 + 1;
                    do
                    {
                        if (*(int*)pEVar8->name == 0 &&
                            g_pc_guess->inventory_items[(short)(int)weaponSlot]->size <= (u8)pEVar1[1].lore[8])
                        {
                            temp = bagIndex + (slotIndex * 5 + 0xF) * 2;
                            // as original: the original's container path falls into the test the empty-slot path jumps to (@116)
                            goto checkFound;
                        }
                        bagIndex = bagIndex + 1;
                        pEVar8 = (EQ_Item*)(pEVar8->name + 4);
                    } while (bagIndex < (int)(u8)pEVar1[1].lore[6]);
                }
                slotIndex = slotIndex + 1;
                invEntry = invEntry + 1;
            } while (slotIndex < 0x1E);
            if ((short)temp != 0 || g_pc_guess->inventory_items[0] == 0)
            {
                EQ_METHOD(4, void, 0x00463651, (void*, short, int, int, int), g_eqmain_guess)((short)(int)weaponSlot, (int)(short)temp, 0, 1);
            }
            else
            {
                strcpy(requestBuf, (const char*)g_pc_guess);
                requestSlot = (short)(int)weaponSlot;
                send_message_guess(g_world_connection_guess, 0x21B2, requestBuf, 0x24, 1);
            }
            if ((short)(int)weaponSlot == 0xD)
            {
                EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, char*), g_display_guess)(g_local_player_guess, 7, EQ_ARRAY(char, 0x0053D860));
            }
            else if ((short)(int)weaponSlot == 0xE || (short)(int)weaponSlot == 0xB)
            {
                EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, char*), g_display_guess)(g_local_player_guess, 8, EQ_ARRAY(char, 0x0053D860));
            }
            pinstEQ_Main_guess->uiDirty_guess = 1;
            EQ_CDECL(void, 0x004A204D, (CharacterProfile*))(g_pc_guess);
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x0055E4FC), 0x111);
        }
    }
    {
    DisarmReply_guess reply;
    reply.result = disarmPacket->success;
    reply.field_4 = disarmPacket->target;
    reply.field_0 = disarmPacket->begger;
    send_message_guess(g_world_connection_guess, 0x20AA, &reply, 0xC, 1);
    }
    return;
    }
    if (result > 0)
    {
        player = EQPlayer_GetPlayerFromID_guess(g_player_top_guess, disarmPacket->target);
        if (player != 0)
        {
            targetName = EQ_METHOD(1, char*, 0x004D02FD, (CEverQuest*, char*), pinstCEverQuest_guess)((char*)player + 1);
            sprintf(message, EQ_ARRAY(char, 0x0055E4E8), targetName);
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(message, 0x10E);
        }
        EQ_METHOD(2, void, 0x004501A1, (void*, int, float), g_pc_guess)(0x10, 1.0f);
        return;
    }
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x0055E4C8), 0x10E);
}

// FUNCTION: TRILOGY 0x004942e4
// Reports the begging result and credits any coins won to the character.
void __cdecl HandleBeggingResult_guess(void* connection, u32 opcode, MsgBeg* msg, u32 size)
{
    typedef void (__fastcall *EQUnknownUpdate_guess)(void*);
    char message[256];
    void* counterObject;
    int* counterValue;
    void* firstObject;
    void* secondObject;
    EQUnknownUpdate_guess update;

    counterObject = EQ_GLOBAL(void*, 0x006E4E44);
    if (counterObject != 0)
    {
        counterValue = (int*)((u8*)counterObject + 0x94);
        if (*counterValue > 0)
            *counterValue = *counterValue - 1;
        else
            *counterValue = 0;
    }

    firstObject = EQ_GLOBAL(void*, 0x006018C0);
    if (firstObject != 0 && *(int*)((u8*)firstObject + 0x10) != 0)
    {
        update = (EQUnknownUpdate_guess)*(u32*)(*(u32*)firstObject + 8);
        update(firstObject);
    }

    secondObject = EQ_GLOBAL(void*, 0x00604984);
    if (secondObject != 0 && *(int*)((u8*)secondObject + 0x10) != 0)
    {
        *(int*)((u8*)secondObject + 0x1E8) = 1;
        secondObject = EQ_GLOBAL(void*, 0x00604984);
        update = (EQUnknownUpdate_guess)*(u32*)(*(u32*)secondObject + 8);
        update(secondObject);
    }

    switch (msg->success)
    {
    case 0:
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(
             EQ_ARRAY(char, 0x0055E514), 0x10E);
        break;
    case 1:
        sprintf(message, EQ_ARRAY(char, 0x0055E540), msg->coins);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x10E);
        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->platinum += msg->coins;
        break;
    case 2:
        sprintf(message, EQ_ARRAY(char, 0x0055E560), msg->coins);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x10E);
        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->gold += msg->coins;
        break;
    case 3:
        sprintf(message, EQ_ARRAY(char, 0x0055E57C), msg->coins);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x10E);
        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->silver += msg->coins;
        break;
    case 4:
        sprintf(message, EQ_ARRAY(char, 0x0055E598), msg->coins);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x10E);
        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->copper += msg->coins;
        break;
    }
}

class StolenEquipment_guess : public EQ_Equipment
{
public:

    // EXTERNAL: TRILOGY 0x00436099 StolenEquipment_guess::StolenEquipment_guess
    StolenEquipment_guess(void);
};

class StolenNote_guess : public EQ_Note
{
public:

    // EXTERNAL: TRILOGY 0x0044FF75 StolenNote_guess::StolenNote_guess
    StolenNote_guess(void);
};

// FUNCTION: TRILOGY 0x00494485
// Pickpocket: warns of and rejects an attempt on us, or credits our stolen coins or item to the inventory.
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
// @early-stop: local new/constructor thunks, deferred stack cleanup, and dummy-EDX setup still differ
void __cdecl msg_steal_guess(void* connection, u32 opcode, u8* msg, u32 size)
{
    class UIUpdate_guess
    {
    public:
        virtual void Unknown0_guess(void) = 0;
        virtual void Unknown1_guess(void) = 0;
        virtual void Refresh_guess(void) = 0;
    };
    char message[256];
    u8 reply[0x810];
    char result;
    int reliability;
    void* firstObject;
    void* secondObject;
    void* item_data;
    char item_type;
    EQ_Item* item;
    EQ_Item* container;
    s16 slot;
    u32 item_size;

    if (*(char*)(msg + 9) == -1)
    {
        *(char*)(msg + 9) = 0;
        reliability = 1;
        EQ_CDECL(int, 0x004A46F6, (int, int))(reliability, 100);
        reply[9] = 0;
        *(u32*)(reply + 0x0C) = 0;
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(
             EQ_ARRAY(char, 0x0055E6BC), 0x0D);
        *((u32*)reply + 1) = *(u32*)msg;
        *(u32*)reply = *((u32*)msg + 1);
        if (*(char*)(reply + 9) == 5)
        {
            if (*((u8*)&msg + 3) == 0)
            {
                send_message_guess(g_world_connection_guess, 0x20AD, reply, 0x136, reliability);
            }
            else
            {
                send_message_guess(g_world_connection_guess, 0x20AD, reply, 0x0E2, reliability);
            }
        }
        else
        {
            send_message_guess(g_world_connection_guess, 0x20AD, reply, 0x12, reliability);
        }
        return;
    }

    if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
    {
        int* counter = (int*)((u8*)EQ_GLOBAL(void*, 0x006E4E44) + 0x94);
        if (*counter > 0)
            *counter = *counter - 1;
        else
            *counter = 0;
    }

    firstObject = EQ_GLOBAL(void*, 0x006018C0);
    if (firstObject != 0 && *(int*)((u8*)firstObject + 0x10) != 0)
    {
        ((UIUpdate_guess*)firstObject)->Refresh_guess();
    }

    secondObject = EQ_GLOBAL(void*, 0x00604984);
    if (secondObject != 0 && *(int*)((u8*)secondObject + 0x10) != 0)
    {
        *(int*)((u8*)secondObject + 0x1E8) = 1;
        secondObject = EQ_GLOBAL(void*, 0x00604984);
        ((UIUpdate_guess*)secondObject)->Refresh_guess();
    }

    result = *(char*)(msg + 9);
    switch (result)
    {
    case 0:
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(
             EQ_ARRAY(char, 0x0055E5B4), 0x10E);
        break;
    case 1:
        sprintf(message, EQ_ARRAY(char, 0x0055E5E0), *(int*)(msg + 0x0C));
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x10E);
        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->platinum += *(int*)(msg + 0x0C);
        break;
    case 2:
        sprintf(message, EQ_ARRAY(char, 0x0055E600), *(int*)(msg + 0x0C));
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x10E);
        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->gold += *(int*)(msg + 0x0C);
        break;
    case 3:
        sprintf(message, EQ_ARRAY(char, 0x0055E61C), *(int*)(msg + 0x0C));
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x10E);
        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->silver += *(int*)(msg + 0x0C);
        break;
    case 4:
        sprintf(message, EQ_ARRAY(char, 0x0055E638), *(int*)(msg + 0x0C));
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x10E);
        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->copper += *(int*)(msg + 0x0C);
        break;
    case 5:
    {
        /* fake match: keep the adjusted packet argument live beside the item-allocation temporary */
        u8* volatile* msg_home = (u8* volatile*)&msg;
        item_data = operator new(0xAC);
        EQ_GLOBAL(void*, 0x005DFBBC) = item_data;
        msg += 0x10;
        memcpy(item_data, msg, 0xAC);
        item_type = *((char*)EQ_GLOBAL(void*, 0x005DFBBC) + 0x81);
        sprintf(message, EQ_ARRAY(char, 0x0055E6A4),
             (const char*)EQ_GLOBAL(void*, 0x005DFBBC));
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x10E);
        if (*(s16*)((u8*)EQ_GLOBAL(void*, 0x005DFBBC) + 0x86) < 0)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(
                 EQ_ARRAY(char, 0x0055E654), 0x10E);
        }
        operator delete(EQ_GLOBAL(void*, 0x005DFBBC));
        EQ_GLOBAL(void*, 0x005DFBBC) = 0;

        if (item_type == 0)
        {
            item_size = 0x124;
            item = (EQ_Item*)new StolenEquipment_guess;
        }
        else
        {
            item_size = 0xD0;
            item = (EQ_Item*)new StolenNote_guess;
        }
        memcpy(item, *msg_home, item_size);
        slot = item->equip_slot;
        if (slot >= 0)
        {
            if (slot < 0x1E)
            {
                firstObject = EQ_GLOBAL(void*, 0x006018C0);
                if (firstObject != 0 && *(int*)((u8*)firstObject + 0x10) != 0)
                {
                    ((UIUpdate_guess*)firstObject)->Refresh_guess();
                }
                secondObject = EQ_GLOBAL(void*, 0x00604984);
                if (secondObject != 0 && *(int*)((u8*)secondObject + 0x10) != 0)
                {
                    *(int*)((u8*)secondObject + 0x1E8) = 1;
                    secondObject = EQ_GLOBAL(void*, 0x00604984);
                    ((UIUpdate_guess*)secondObject)->Refresh_guess();
                }
                if (item->equip_slot == 0)
                    *((u8*)g_display_guess + 0x3C) = 1;
                EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[item->equip_slot] = item;
            }
            else
            {
                container = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)
                    ->inventory_items[(s16)((slot - 0x1E) / 10)];
                if (container != 0 && container->type == 1)
                {
                    slot = (s16)((slot - 0x1E) % 10);
                    *((EQ_Item**)((u8*)container + 0xAC) + slot) = item;
                }
            }
        }
        break;
    }
    }

    EQ_CDECL(void, 0x004A2816, (CharacterProfile*))
        (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
}

// FUNCTION: TRILOGY 0x00494884
// @early-stop: insertion store block is emitted before the send block, shifting normalized branch targets.
char msg_move_item_guess(EQ_Item* item, char skipDuplicateCheck)
{
    int* entryPtr;
    int tmp;
    int moveMsg[3];
    char duplicate;
    char added;

    added = 0;
    if (item != 0)
    {
        duplicate = 0;
        if (skipDuplicateCheck == '\0')
        {
            if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8) != 0 &&
                EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0] != 0 &&
                item->item_nr ==
                    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0]->item_nr)
            {
                duplicate = 1;
            }
            else
            {
                entryPtr = EQ_ARRAY(int, 0x006B67B4);
                do
                {
                    tmp = *entryPtr;
                    if (tmp != 0 && *(s16*)(tmp + 0x86) < 30000 &&
                        *(s16*)(tmp + 0x82) == item->item_nr)
                    {
                        duplicate = 1;
                        break;
                    }
                    entryPtr = entryPtr + 1;
                } while ((int)entryPtr < 0x006B6844);
            }
        }
        if (duplicate == 0)
        {
            tmp = 0;
            entryPtr = EQ_ARRAY(int, 0x006B67B4);
            do
            {
                if (*entryPtr != 0)
                {
                    entryPtr = entryPtr + 1;
                    tmp = tmp + 1;
                }
                else
                {
                    EQ_ARRAY(EQ_Item*, 0x006B67B4)[tmp] = item;
                    added = 1;
                    break;
                }
            } while ((int)entryPtr < 0x006B6844);
            return added;
        }
        else
        {
            moveMsg[0] = (int)item->equip_slot;
            moveMsg[1] = -1;
            moveMsg[2] = 0;
            send_message_guess(g_world_connection_guess, 0x212C, moveMsg, 0x0c, 1);
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

// FUNCTION: TRILOGY 0x0049494e
// Handles a character's level-up and its related progression updates.
// ORIGINAL FILE: unknown (subsystem from the call graph)
// __cdecl message handler, OP_LevelUpdate (MsgLevelUpdate). Only a level above the profile's counts (the
// dwords are compared signed): +5 training points when the new level is above old_level, then level,
// highest_level (raised only), exp, the local spawn's level and its sneak modifier, the UI redraw flag and
// sound 0x8B.
void msg_level_update_guess(void* connection, u32 opcode, MsgLevelUpdate* msg, u32 size)
{
    if (g_pc_guess == NULL)
        return;
    if (g_pc_guess->level >= (int)msg->level)
        return;
    if ((int)msg->level > (int)msg->old_level)
        g_pc_guess->training_points += 5;
    g_pc_guess->level = (u8)msg->level;
    if ((int)msg->level > g_pc_guess->highest_level)
        g_pc_guess->highest_level = (u8)msg->level;
    g_pc_guess->exp = msg->exp;
    if (g_local_player_guess != NULL)
    {
        g_local_player_guess->level = g_pc_guess->level;
        EQPlayer_InitSneakMod_guess(g_local_player_guess);
    }
    *(s32*)(g_eqmain_guess + 0xEB8) = 1;
    if (g_sound_manager_guess != NULL)
        SoundManager_WavePlay_guess(g_sound_manager_guess, 0x8B, 0);
}

// FUNCTION: TRILOGY 0x004949f1
// Processes successful-hit messages and applies damage to the local character.
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
void msg_successful_hit(u32* hit_message)
{
    typedef void (__fastcall *NoArgMethod_guess)(void*);
    u32 local_spawn_id;
    s32 damage;
    bool report_hit;
    EQPlayer* attacker;
    float distance;
    void* hit_reaction;

    local_spawn_id = g_local_player_guess->spawn_id;
    report_hit = true;
    if (hit_message[1] == local_spawn_id || hit_message[0] == local_spawn_id)
    {
        if (hit_message[0] == local_spawn_id &&
            (((char*)hit_message)[8] == (char)0xE7 || (s32)hit_message[3] > 0 ||
             ((char*)hit_message)[8] == (char)0xE7) &&
            EQ_GLOBAL(void*, 0x006E4E44) != NULL)
        {
            damage = (s32)hit_message[3];
            if (damage <= 0)
            {
                if (damage >= 0)
                    goto report_hit_result;
            }
            else
            {
                if (pinstEQ_Main_guess->screenMode_u8_guess == 5 &&
                    g_local_player_guess->stand_state == 'n')
                    EQ_METHOD(1, void, 0x004B45CB, (EQPlayer*, int), g_local_player_guess)(100);

                hit_reaction = EQ_GLOBAL(void*, 0x006E4DF8);
                if (hit_reaction != NULL && *(int*)((char*)hit_reaction + 0x10) != 0)
                    (*(NoArgMethod_guess*)(*(u32*)hit_reaction + 8))(hit_reaction);

                if (((char*)hit_message)[8] == (char)0xE7)
                    goto report_hit_result;
                damage = (s32)hit_message[3];
            }

            EQ_METHOD(2, void, 0x0041976F, (CharacterProfile*, int, int), g_pc_guess)(-damage, 0);
            g_local_player_guess->cur_hp =
                EQ_METHOD(1, int, 0x004199C0, (CharacterProfile*, int), g_pc_guess)(0);
        }
    }
    else
    {
        attacker = EQ_METHOD(1, EQPlayer*, 0x004B1416, (EQPlayer*, int),
            EQ_GLOBAL(EQPlayer*, 0x006E4D58))((int)hit_message[0]);
        distance = EQ_METHOD(3, float, 0x00412A4E, (void*, EQPlayer*, EQPlayer*, float),
            g_display_guess)(g_local_player_guess, attacker, 1.0f);
        if (EQ_GLOBAL(float, 0x0053568C) < distance)
            report_hit = false;
    }

report_hit_result:
    EQ_METHOD(2, void, 0x004C5E50, (void*, u32*, bool), g_everquest_guess)(hit_message, report_hit);
}

// FUNCTION: TRILOGY 0x00494af1
// Adds a received container item to an empty character inventory or bank slot.
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
// @early-stop: the inventory and bank installs are two copies (the original shares one tail via goto); the cleanup object takes slot -0x10 so this and the pointer swap slots, and its release store and end-of-function destructor call add instructions; the window arms don't cache the constant 1 in EDI.
// __thiscall item handler (thiscall adapter). Copies the container, clears its live contents and open
// flag, and installs it in inventory or the bank when the requested slot is empty.
s32 __fastcall msgStartIcon(void* self, int unusedEdx, EQ_Container* item)
{
    EQ_Container* copiedItem;
    s32 message[0x17];
    s32 state;

    copiedItem = (EQ_Container*)new ReceivedContainerAllocation_guess;
    memcpy(copiedItem, item, 0xDC);

    state = EQ_GLOBAL(int, 0x006B64E8);
    if (EQ_GLOBAL(int, 0x006B70E0) == 0 && state == 0)
    {
        s16 itemNr = copiedItem->item.item_nr;
        if (itemNr == 0)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(EQ_ARRAY(char, 0x0055E314), 0x0D);
            if (copiedItem != NULL)
            {
                EQ_METHOD(0, void, 0x0043405A, (EQ_Container*), copiedItem)();
                operator delete(copiedItem);
            }
            message[0] = -2;
            message[1] = -2;
            message[2] = 0x0E;
        }
        else
        {
            if (*(int*)((char*)self + 4) == 5 && EQ_ARRAY(int, 0x006B32B4)[copiedItem->item.icon_nr] == 0)
            {
                char text[0x200];
                sprintf(text, EQ_ARRAY(char, 0x0055E2F8), (int)itemNr);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(text, 0x0D);
                copiedItem->item.icon_nr = 500;
            }

            copiedItem->open = 0;
            for (int i = 0; i != 10; ++i)
                copiedItem->contents[i] = NULL;

            void* window = EQ_GLOBAL(void*, 0x006018C0);
            if (window != NULL && *(int*)((char*)window + 0x10) != 0)
                ((void (__fastcall*)(void*))(*(u32**)window)[2])(window);

            window = EQ_GLOBAL(void*, 0x00604984);
            if (window != NULL && *(int*)((char*)window + 0x10) != 0)
            {
                *(int*)((char*)window + 0x1E8) = 1;
                window = EQ_GLOBAL(void*, 0x00604984);
                ((void (__fastcall*)(void*))(*(u32**)window)[2])(window);
            }

            s16 slot = copiedItem->item.equip_slot;
            if (slot < 0x1E)
            {
                EQ_Item** slotPointer = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items + slot;
                if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[slot] == NULL)
                {
                    *slotPointer = (EQ_Item*)copiedItem;
                    EQ_CDECL(void, 0x004A2816, (CharacterProfile*))(EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
                    if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0] != NULL)
                        *(u8*)((char*)g_display_guess + 0x3C) = 1;
                    return 1;
                }
                if (copiedItem != NULL)
                {
                    EQ_METHOD(0, void, 0x0043405A, (EQ_Container*), copiedItem)();
                    operator delete(copiedItem);
                }
                message[0] = -2;
                message[1] = -2;
                message[2] = 0x0F;
            }
            else if (slot < 2000 || slot >= 2009)
            {
                if (copiedItem != NULL)
                {
                    EQ_METHOD(0, void, 0x0043405A, (EQ_Container*), copiedItem)();
                    operator delete(copiedItem);
                }
                message[0] = -2;
                message[1] = -2;
                message[2] = 0x11;
            }
            else
            {
                s16 bankSlot = (s16)(slot - 2000);
                EQ_Item** slotPointer = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->bank_items + bankSlot;
                if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->bank_items[bankSlot] == NULL)
                {
                    *slotPointer = (EQ_Item*)copiedItem;
                    EQ_CDECL(void, 0x004A2816, (CharacterProfile*))(EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
                    if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0] != NULL)
                        *(u8*)((char*)g_display_guess + 0x3C) = 1;
                    return 1;
                }
                if (copiedItem != NULL)
                {
                    EQ_METHOD(0, void, 0x0043405A, (EQ_Container*), copiedItem)();
                    operator delete(copiedItem);
                }
                message[0] = -2;
                message[1] = -2;
                message[2] = 0x10;
            }
        }
    }
    else
    {
        message[2] = 1000;
        message[0] = -2;
        message[1] = -2;
        if (state != 0)
            message[2] = 2000;
    }

    send_message_guess(g_world_connection_guess, 0x2147, message, 0x5C, 1);
    return 0;
}

// FUNCTION: TRILOGY 0x00494d6b
// Places a received note or book in its inventory, bag or bank slot; tells the server when placement fails.
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
// @early-stop: the EH ctor/dtor and the release store are extra calls and stores; VC6 spills errorCode and folds 30 into EDI, so the bag arm and copy/0xd0 registers still differ
// __thiscall item handler (thiscall adapter). Copies a note and installs it in inventory,
// a bag, the bank, or a bank bag when the destination is empty.
/* fake match: MessageFrame_guess shares the frame slot with the cleanup object so VC6 places the message where the original's new temp and message are; the gotos reuse the original's shared store, tail and failure blocks. */
s32 __fastcall msgStartInotes(void* self, int unusedEdx, EQ_Note* item)
{
    class AllocationCleanup_guess
    {
    public:
        EQ_Item* address;
        AllocationCleanup_guess(EQ_Item* value) : address(value) {}
        ~AllocationCleanup_guess()
        {
            if (address != NULL)
                operator delete(address);
        }
    };

    struct MessageFrame_guess
    {
        AllocationCleanup_guess cleanup;
        s32 message[0x17];
        MessageFrame_guess(EQ_Item* value) : cleanup(value) {}
    } frame((EQ_Item*)operator new(0xD0));

    s16 slot;
    s16 bagSlot;
    s16 parentSlot;
    s16 itemNr;
    int state;
    int i;
    EQ_Item* copiedItem;
    EQ_Item* parentItem;
    char* childPointer;
    int errorCode;

    copiedItem = NULL;
    if (frame.cleanup.address != NULL)
        copiedItem = EQ_METHOD(0, EQ_Item*, 0x0044FF75, (EQ_Item*), frame.cleanup.address)();
    frame.cleanup.address = NULL;
    memcpy(copiedItem, item, 0xD0);

    state = EQ_GLOBAL(int, 0x006B64E8);
    if (EQ_GLOBAL(int, 0x006B70E0) == 0 && state == 0)
    {
        itemNr = copiedItem->item_nr;
        errorCode = 0x0D;
        if (itemNr == 0)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(EQ_ARRAY(char, 0x0055E314), 0x0D);
            if (copiedItem != NULL)
            {
                EQ_METHOD(0, void, 0x0040101D, (EQ_Item*), copiedItem)();
                operator delete(copiedItem);
            }
            frame.message[2] = 8;
            frame.message[0] = -2;
            frame.message[1] = -2;
        }
        else
        {
            if (*(int*)((char*)self + 4) == 5 && EQ_ARRAY(int, 0x006B32B4)[copiedItem->icon_nr] == 0)
            {
                char text[0x200];
                sprintf(text, EQ_ARRAY(char, 0x0055E6E8), itemNr);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(text, errorCode);
                copiedItem->icon_nr = 500;
            }

            void* window = EQ_GLOBAL(void*, 0x006018C0);
            if (window != NULL && *(int*)((char*)window + 0x10) != 0)
                ((void (__fastcall*)(void*))(*(u32**)window)[2])(window);

            window = EQ_GLOBAL(void*, 0x00604984);
            if (window != NULL && *(int*)((char*)window + 0x10) != 0)
            {
                *(int*)((char*)window + 0x1E8) = 1;
                window = EQ_GLOBAL(void*, 0x00604984);
                ((void (__fastcall*)(void*))(*(u32**)window)[2])(window);
            }

            slot = copiedItem->equip_slot;
            if (slot < 0x1E)
            {
                EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[slot] = copiedItem;
                goto item_inserted;
            }
            if (slot <= 0x154)
            {
                errorCode = 10;
                i = slot - 0x1E;
                parentSlot = (s16)(i / 10);
                bagSlot = (s16)(i % 10);
                if (parentSlot == 0x1E)
                    parentSlot = 0;
                parentItem = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[parentSlot];
                if (parentItem == NULL || parentItem->type != 1)
                {
                    if (copiedItem != NULL)
                    {
                        EQ_METHOD(0, void, 0x0040101D, (EQ_Item*), copiedItem)();
                        operator delete(copiedItem);
                    }
                    frame.message[2] = 9;
                    frame.message[0] = -2;
                    frame.message[1] = -2;
                }
                else
                {
                    if (parentItem != NULL && (childPointer = (char*)parentItem + 0xAC + bagSlot * 4, *(int*)childPointer == 0))
                        goto store_item;
                fail_item:
                    if (copiedItem != NULL)
                    {
                        EQ_METHOD(0, void, 0x0040101D, (EQ_Item*), copiedItem)();
                        operator delete(copiedItem);
                    }
                    frame.message[2] = errorCode;
                    frame.message[0] = -2;
                    frame.message[1] = -2;
                }
            }
            else
            {
                if (2000 <= slot && slot < 0x7D9)
                {
                    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->bank_items[(s16)(slot - 2000)] = copiedItem;
                    goto item_inserted;
                }
                if (slot < 0x7EE)
                    goto fail_item;
                i = (int)(short)(slot - 2000) - 0x1E;
                parentSlot = (s16)(i / 10);
                bagSlot = (s16)(i % 10);
                parentItem = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->bank_items[parentSlot];
                if (parentItem == NULL || parentItem->type != 1)
                {
                    if (copiedItem != NULL)
                    {
                        EQ_METHOD(0, void, 0x0040101D, (EQ_Item*), copiedItem)();
                        operator delete(copiedItem);
                    }
                    frame.message[2] = 0x0B;
                    frame.message[0] = -2;
                    frame.message[1] = -2;
                }
                else
                {
                    if (parentItem != NULL && (childPointer = (char*)parentItem + 0xAC + bagSlot * 4, *(int*)childPointer == 0))
                    {
                    store_item:
                        *(EQ_Item**)childPointer = copiedItem;
                    item_inserted:
                        EQ_CDECL(void, 0x004A2816, (CharacterProfile*))(EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
                        if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0] != NULL)
                            *(u8*)((char*)g_display_guess + 0x3C) = 1;
                        return 1;
                    }
                    if (copiedItem != NULL)
                    {
                        EQ_METHOD(0, void, 0x0040101D, (EQ_Item*), copiedItem)();
                        operator delete(copiedItem);
                    }
                    frame.message[2] = 0x0C;
                    frame.message[0] = -2;
                    frame.message[1] = -2;
                }
            }
        }
    }
    else
    {
        frame.message[2] = 1000;
        frame.message[0] = -2;
        frame.message[1] = -2;
        if (state != 0)
            frame.message[2] = 2000;
    }

    send_message_guess(g_world_connection_guess, 0x2147, frame.message, 0x5C, 1);
    return 0;
}

class MsgSndIeqEquipment_guess : public EQ_Equipment
{
public:
    // EXTERNAL: TRILOGY 0x00436099 MsgSndIeqEquipment_guess::MsgSndIeqEquipment_guess
    MsgSndIeqEquipment_guess(void);
};

// FUNCTION: TRILOGY 0x0049509b
// Installs a received equipment item in inventory, bag or bank; deletes bad/unplaceable ones and reports a code
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
// @early-stop: `new` calls a COMDAT constructor shim, not 0x00436099 directly; EBX never holds the constants 0xd and 1 (the original's sVar7 register), so those pushes/compares differ; the delete cleanup is not deferred into the send's add esp.
s32 __fastcall msg_snd_ieq(void* self, int unusedEdx, EQ_Equipment* item)
{
    // The item copy is a `new` whose constructor runs 0x00436099. The new-expression keeps VC6's EH frame
    // around that call, as the original does, and takes the constructed pointer from EAX.
    int state;
    int result;
    int inserted;
    s16 slot;
    s16 parentSlot;
    EQ_Equipment* copiedItem;
    EQ_Item* parentItem;
    char* childPointer;
    s32 message[0x17];
    char text[0x200];

    copiedItem = new MsgSndIeqEquipment_guess;
    memcpy(copiedItem, item, 0x124);

    state = EQ_GLOBAL(int, 0x006B64E8);
    if (EQ_GLOBAL(int, 0x006B70E0) == 0 && state == 0)
    {
        result = 1;
        inserted = 0;
        if (copiedItem->item_type == 0x0D || copiedItem->item.item_nr < 1000)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(EQ_ARRAY(char, 0x0055E314), 0x0D);
            operator delete(copiedItem);
            message[2] = 1;
            message[0] = -2;
            message[1] = -2;
        }
        else
        {
            if (*(int*)((char*)self + 4) == 5 &&
                EQ_ARRAY(int, 0x006B32B4)[copiedItem->item.icon_nr] == 0)
            {
                sprintf(text, EQ_ARRAY(char, 0x0055E2DC), (int)copiedItem->item.item_nr);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(text, 0x0D);
                copiedItem->item.icon_nr = 500;
            }

            if (copiedItem->click_effect_type == 0 ||
                (EQ_GLOBAL(EQ_Equipment*, 0x005DFBBC) = copiedItem,
                 EQ_CDECL(int, 0x004A1D83, (EQ_Equipment*))(copiedItem)) == 0 ||
                copiedItem->charges > 0)
            {
                void* window = EQ_GLOBAL(void*, 0x006018C0);
                if (window != NULL && *(int*)((char*)window + 0x10) != 0)
                    ((void (__fastcall*)(void*))(*(u32**)window)[2])(window);

                window = EQ_GLOBAL(void*, 0x00604984);
                if (window != NULL && *(int*)((char*)window + 0x10) != 0)
                {
                    *(int*)((char*)window + 0x1E8) = 1;
                    ((void (__fastcall*)(void*))(*(u32**)EQ_GLOBAL(void*, 0x00604984))[2])(EQ_GLOBAL(void*, 0x00604984));
                }

                slot = copiedItem->item.equip_slot;
                if (slot < 0x1E)
                {
                    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[slot] = (EQ_Item*)copiedItem;
                    inserted = 1;
                }
                else if (slot <= 0x154)
                {
                    parentSlot = (short)((slot - 0x1E) / 10);
                    state = (slot - 0x1E) % 10;
                    if (parentSlot == 0x1E)
                        parentSlot = 0;
                    parentItem = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[parentSlot];
                    if (parentItem == NULL || parentItem->type != 1)
                    {
                        operator delete(copiedItem);
                        message[0] = -2;
                        message[1] = -2;
                        message[2] = 3;
                    }
                    else
                    {
                        if (parentItem != NULL && (childPointer = (char*)parentItem + 0xAC + state * 4, *(int*)childPointer == 0))
                        {
                            *(EQ_Item**)childPointer = (EQ_Item*)copiedItem;
                            inserted = 1;
                        }
                        else
                        {
                            operator delete(copiedItem);
                            message[0] = -2;
                            message[1] = -2;
                            message[2] = 4;
                        }
                    }
                }
                else
                {
                    if (2000 <= slot && slot < 0x7D9)
                    {
                        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->bank_items[(short)(slot - 2000)] = (EQ_Item*)copiedItem;
                        inserted = 1;
                    }
                    else if (slot >= 0x7EE)
                    {
                        state = (short)(((short)(slot - 2000) - 0x1E) % 10);
                        parentSlot = (short)(((short)(slot - 2000) - 0x1E) / 10);
                        parentItem = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->bank_items[parentSlot];
                        if (parentItem == NULL || parentItem->type != 1)
                        {
                            operator delete(copiedItem);
                            message[0] = -2;
                            message[1] = -2;
                            message[2] = 5;
                        }
                        else
                        {
                            if (parentItem != NULL && (childPointer = (char*)parentItem + 0xAC + state * 4, *(int*)childPointer == 0))
                            {
                                *(EQ_Item**)childPointer = (EQ_Item*)copiedItem;
                                // as original: the original's bank-parent insert falls through into the shared item_inserted tail
                                goto item_inserted;
                            }
                            else
                            {
                                operator delete(copiedItem);
                                message[0] = -2;
                                message[1] = -2;
                                message[2] = 6;
                            }
                        }
                    }
                    else
                    {
                        operator delete(copiedItem);
                        message[0] = -2;
                        message[1] = -2;
                        message[2] = 7;
                    }
                }
            }
            else
            {
                sprintf(text, EQ_ARRAY(char, 0x0055E704), (int)copiedItem->item.item_nr);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(text, 0x0D);
                operator delete(copiedItem);
                message[0] = -2;
                message[1] = -2;
                message[2] = 2;
            }
        }
        /* fake match: the item_inserted tail is spelled out twice, here and at the label below; VC6 merges the two copies into one block, and with only this copy the tail lands after the first insert path (structural 0.7869, not 0.8233) */
        if (inserted != 0)
        {
            EQ_CDECL(void, 0x004A2816, (CharacterProfile*))(EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
            EQ_CDECL(void, 0x004A204D, (CharacterProfile*))(EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
            if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0] != NULL)
                *(u8*)((char*)g_display_guess + 0x3C) = 1;
            return result;
        }
        send_message_guess(g_world_connection_guess, 0x2147, message, 0x5C, 1);
    }
    else
    {
        message[2] = 1000;
        message[0] = -2;
        message[1] = -2;
        if (state != 0)
            message[2] = 2000;
        send_message_guess(g_world_connection_guess, 0x2147, message, 0x5C, 1);
    }
    result = 0;
    return result;

item_inserted:
    EQ_CDECL(void, 0x004A2816, (CharacterProfile*))(EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
    EQ_CDECL(void, 0x004A204D, (CharacterProfile*))(EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
    if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0] != NULL)
        *(u8*)((char*)g_display_guess + 0x3C) = 1;
    return result;
}

// FUNCTION: TRILOGY 0x00495456
// Loads server item data into the client, including containers and notes.
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
s32 __fastcall HandleCompressedItems(void* self, int unusedEdx, MsgCompressedItems* msg, u32 size)
{
    s16 opcode;
    u32 decompressedSize;
    EQ_Equipment* item;

    decompressedSize = EQ_CDECL(u32, 0x0049100C, (void*, void*, u32, u32))
        ((char*)msg + 2, EQ_ARRAY(void, 0x005963B0), size - 2, 0x49800);
    if (decompressedSize != msg->count * 0x126)
        msg->count = (s16)(decompressedSize / 0x126);
    size = 0;
    if (0 < msg->count)
    {
        item = EQ_ARRAY(EQ_Equipment, 0x005963B2);
        do
        {
            opcode = *(s16*)((char*)item - 2);
            if (opcode == 0x2164)
                EQ_METHOD(1, s32, 0x0049509B, (void*, EQ_Equipment*), self)(item);
            else if (opcode == 0x2166)
                EQ_METHOD(1, s32, 0x00494AF1, (void*, EQ_Container*), self)((EQ_Container*)item);
            else if (opcode == 0x2165)
                EQ_METHOD(1, s32, 0x00494D6B, (void*, EQ_Note*), self)((EQ_Note*)item);
            size = size + 1;
            item = (EQ_Equipment*)((char*)item + 0x126);
        } while ((int)size < (int)msg->count);
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004954f6
// Keeps the target spawn's health in sync with the server.
// ORIGINAL FILE: unknown (subsystem from the call graph)
// __cdecl, OP_HPUpdate (MsgHPUpdate; max_hp is not read). Sets the spawn's current hp; for the local player
// also the profile's (as a short) and the redraw flag of the main UI object, when the profile exists.
void msg_hp_update_guess(MsgHPUpdate* msg)
{
    EQPlayer* player = EQPlayer_GetPlayerFromID_guess(g_player_top_guess, msg->spawn_id);

    if (player != NULL)
    {
        player->cur_hp = msg->cur_hp;
        if (player == g_local_player_guess && g_pc_guess != NULL)
        {
            g_pc_guess->cur_hp = (s16)msg->cur_hp;
            *(s32*)(g_eqmain_guess + 0xEB8) = 1;
        }
    }
}

// FUNCTION: TRILOGY 0x0049553f
// Handles a successful skill increase, including the raw 100-entry profile byte range used by the client.
void HandleSkillIncrease_guess(const s32* message)
{
    char text[256];

    if (g_pc_guess != NULL)
    {
        if (((int)(((s32)(*message)) >= 0 && 100 > ((s32)(*message)))))
        {
            g_pc_guess->skills[((s32)(*message))] = (u8)message[1];
            sprintf(text, (const char*)0x0055E72C,
                g_skill_records_guess[*message], g_pc_guess->skills[*message]);
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(text, 0x10E);
        }
        else
        {
        }
    }
}

// FUNCTION: TRILOGY 0x004955b1
// Clears the active poison-result UI fields, then prints the packet's full-width outcome flag.
void HandlePoisonApplicationResult_guess(const s32* message)
{
    if (g_eqmain_guess != NULL)
    {
        *(s32*)(g_eqmain_guess + 0xE5C) = 0;
        *(s32*)(g_eqmain_guess + 0x94) = 0;
        *(s32*)(g_eqmain_guess + 0xEB8) = 1;
    }

    if (message[1] != 0)
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x0055E770), 0x10E);
    else
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x0055E750), 0x10E);
}

// FUNCTION: TRILOGY 0x00495602
// Prints a directional chat warning when another spawn yells for help.
void msg_yell(const u32* yellInfo)
{
    float h2;
    EQPlayer* targetSpawn;
    char temp[256];
    char directionPhrase[128];
    char side[16];
    float signedBearingDelta;
    float bearingDelta;

    targetSpawn = EQ_METHOD(1, EQPlayer*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))((int)*yellInfo);
    if (targetSpawn == 0)
        return;
    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C) == 0)
        return;

    h2 = EQ_CDECL(float, 0x004A47F9, (float, float, float, float))
        (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y,
         EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x,
         targetSpawn->y, targetSpawn->x);
    EQ_METHOD(7, float, 0x004129F3, (void*, float, float, float, float, float, float, float), g_display_guess)(
         EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y,
         EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x,
         EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->z,
         targetSpawn->y, targetSpawn->x, targetSpawn->z, 0.0f);
    bearingDelta = EQ_METHOD(3, float, 0x004081A3, (void*, float, float, float*), g_display_guess)(
         EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->heading, h2, &signedBearingDelta);

    if (((EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->heading < h2) && (signedBearingDelta < 0.0f)) ||
        ((h2 < EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->heading) && (signedBearingDelta > 0.0f)))
        strcpy(side, (char*)0x0054242C);
    else
        strcpy(side, (char*)0x00542424);

    if (bearingDelta >= 0.0f && bearingDelta <= 32.0f)
        strcpy(directionPhrase, (char*)0x0055E810);
    else if (bearingDelta > 32.0f && bearingDelta <= 96.0f)
        sprintf(directionPhrase, (char*)0x0055E7F4, side);
    else if (bearingDelta > 96.0f && bearingDelta <= 160.0f)
        sprintf(directionPhrase, (char*)0x0055E7DC, side);
    else if (bearingDelta > 160.0f && bearingDelta <= 224.0f)
        sprintf(directionPhrase, (char*)0x0055E7C0, side);
    else if (bearingDelta > 224.0f && bearingDelta <= 256.0f)
        strcpy(directionPhrase, (char*)0x0055E7B4);

    sprintf(temp, (const char*)0x0055E798, (char*)targetSpawn + 1, directionPhrase);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(temp, 0x11A);
    return;
}

// FUNCTION: TRILOGY 0x00495808
// Applies a positive local ability score change or reports a remote operation result.
void msg_ability_change(const u32* message)
{
    EQPlayer* localPlayer;
    u32 selector;
    u32 statOffset = 0;
    s32 value;
    const char* text;

    localPlayer = *(EQPlayer* volatile*)0x006E4D8C;
    if (*(const volatile u32*)message == *(const volatile u32*)((u8*)localPlayer + 0x80))
    {
        value = *(const volatile s32*)((const u8*)message + 8);
        if (value <= 0)
            return;
        selector = *(const volatile u32*)((const u8*)message + 4);
        switch (selector)
        {
        case 1: statOffset = 0x7C; break;
        case 2: statOffset = 0x79; break;
        case 3: statOffset = 0x7A; break;
        case 4: statOffset = 0x7B; break;
        case 5: statOffset = 0x78; break;
        case 6: statOffset = 0x77; break;
        case 7: statOffset = 0x7D; break;
        default: return;
        }
        *(volatile u8*)((u32)*(CharacterProfile* volatile*)0x006E4DA8 + statOffset) =
            *((const volatile u8*)((const u8*)message + 8));
        return;
    }

    if (*(const volatile s32*)((const u8*)message + 8) < 0)
        text = (const char*)0x0055E878;
    else
        text = (const char*)0x0055E828;
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), *(CEverQuest* volatile*)0x006EFE2C)(text, 0x0F);
}

// FUNCTION: TRILOGY 0x004958b9
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
// Closes active UI windows, then opens the sacrifice confirmation for the local player.
void msg_sacrifice(const u32* message)
{
    typedef void (__fastcall *WindowAction_guess)(void*);
    void* dialog;
    void* window;
    u8* prompt;
    u8* display;
    u32* vtable;
    EQPlayer* sender;
    char name[64];

    dialog = EQ_GLOBAL(void*, 0x006018C0);
    if (dialog != NULL && *(s32*)((u8*)dialog + 0x10) != 0)
    {
        vtable = *(u32**)dialog;
        ((WindowAction_guess)vtable[2])(dialog);
    }

    window = EQ_GLOBAL(void*, 0x00604984);
    if (window != NULL && *(s32*)((u8*)window + 0x10) != 0)
    {
        *(u32*)((u8*)window + 0x1E8) = 1;
        window = EQ_GLOBAL(void*, 0x00604984);
        vtable = *(u32**)window;
        ((WindowAction_guess)vtable[2])(window);
    }

    prompt = (u8*)EQ_GLOBAL(void*, 0x006E4E20);
    if (prompt != NULL && *(s32*)(prompt + 0x10) == 0 &&
        message[1] == g_local_player_guess->spawn_id)
    {
        *(u8*)(prompt + 0x17C) = 0x14;
        prompt = (u8*)EQ_GLOBAL(void*, 0x006E4E20);
        vtable = *(u32**)prompt;
        ((WindowAction_guess)vtable[1])(prompt);

        sender = EQPlayer_GetPlayerFromID_guess(g_player_top_guess, (int)message[0]);
        if (sender == NULL)
            strcpy(name, (const char*)0x0055E910);
        else
            strcpy(name, (const char*)sender + 1);

        sprintf((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x1B9,
             (const char*)0x0055E8AC, name);

        display = (u8*)g_display_guess;
        prompt = (u8*)EQ_GLOBAL(void*, 0x006E4E20);
        *(u32*)(prompt + 0x178) = *(u32*)(display + 0xBC) + 3600000u;
    }
}

// FUNCTION: TRILOGY 0x00495996
// Handles the local character's translocation, including reviewing or accepting the request.
// __cdecl, OP_Translocate (MsgTranslocate). Unconfirmed: closes the active windows and opens the
// translocate confirmation (to the bind point for spells 0x58E / 0x536). Confirmed: puts the local player
// at the bind point or the message position, stops it and, in the same zone, settles it on the floor
// (-1e27 = no floor); in another zone calls ZonePC.
void TeleportPC_guess(MsgTranslocate* msg)
{
    typedef void (__fastcall *WindowAction_guess)(void*);
    char local[80];
    void* window;
    float floorZ;

    if (msg->confirmed != 1)
    {
        window = EQ_GLOBAL(void*, 0x006018C0);
        if (window != NULL && *(s32*)((u8*)window + 0x10) != 0)
            ((WindowAction_guess)(*(u32**)window)[2])(window);

        window = EQ_GLOBAL(void*, 0x00604984);
        if (window != NULL && *(s32*)((u8*)window + 0x10) != 0)
        {
            *(u32*)((u8*)window + 0x1E8) = 1;
            window = EQ_GLOBAL(void*, 0x00604984);
            ((WindowAction_guess)(*(u32**)window)[2])(window);
        }

        if (msg->spell_id == 0x58E || msg->spell_id == 0x536)
        {
            sprintf((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x1B9, (const char*)0x0055E92C);
        }
        else
        {
            EQ_METHOD(2, void, 0x004BEA23, (void*, MsgTranslocate*, char*), EQ_GLOBAL(void*, 0x006E4D54))(msg, local);
            sprintf((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x1B9, (const char*)0x0055E96C,
                 EQ_METHOD(1, char*, 0x004D02FD, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(msg->caster),
                 local);
        }

        *(u8*)((u8*)EQ_GLOBAL(void*, 0x006E4E20) + 0x17C) = 0x19;
        window = EQ_GLOBAL(void*, 0x006E4E20);
        ((WindowAction_guess)(*(u32**)window)[1])(window);

        *(u32*)((u8*)EQ_GLOBAL(void*, 0x006E4E20) + 0x178) =
            g_cdisplay_guess->time_ms_guess + 20000;
    }
    else
    {
        g_local_player_guess->actor_info->ride_target_guess = 0;
        g_pc_guess->boat[0] = '\0';
        if (msg->spell_id == 0x536 || msg->spell_id == 0x58E)
        {
            g_local_player_guess->y = g_pc_guess->bind_location[0][1];
            g_pc_guess->y = g_local_player_guess->y;
            g_local_player_guess->x = g_pc_guess->bind_location[1][1];
            g_pc_guess->x = g_local_player_guess->x;
            g_local_player_guess->z = g_pc_guess->bind_location[2][1];
        }
        else
        {
            g_local_player_guess->y = msg->y;
            g_pc_guess->y = g_local_player_guess->y;
            g_local_player_guess->x = msg->x;
            g_pc_guess->x = g_local_player_guess->x;
            g_local_player_guess->z = msg->z;
        }
        g_pc_guess->z = g_local_player_guess->z;
        g_local_player_guess->dy = 0.0f;
        g_local_player_guess->dx = 0.0f;
        g_local_player_guess->dz = 0.0f;
        g_local_player_guess->delta_heading = 0.0f;
        g_local_player_guess->unk_0x5C = 0.0f;
        g_local_player_guess->speed = 0.0f;
        EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), g_display_guess)(g_local_player_guess->y,
             g_local_player_guess->x, g_local_player_guess->z, 0x89);

        if (_strcmpi(msg->zone, g_local_player_guess->zone) == 0)
        {
            floorZ = CDisplay_GetPlayerFloorHeight_guess(g_display_guess, g_local_player_guess,
                g_local_player_guess->y, g_local_player_guess->x,
                g_local_player_guess->z + 1.0f, 1);
            if (floorZ != -1.0e27f)
            {
                g_local_player_guess->actor_info->floor_z = floorZ;
                g_local_player_guess->z = floorZ + g_local_player_guess->height_offset;
            }
            CDisplay_UpdatePlayerActor_guess(g_display_guess, g_local_player_guess);
        }
        else
        {
            EQ_METHOD(3, void, 0x004DC1B5, (CEverQuest*, MsgTranslocate*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(msg, (const char*)0x0055E918, 0);
        }
    }
}

// FUNCTION: TRILOGY 0x00495C57
// Presents an incoming spell or resurrection offer to the local character.
// @early-stop: resurrect block laid out after the cast block (VC6 jumps to it from all three || operands); two EQ_THISCALL trimName calls add a dummy EDX xor
void HandleCastOrResurrectionOffer_guess(void* message)
{
    typedef void (__fastcall *WindowAction_guess)(void*);
    void* window;
    void* prompt;
    void* display;
    u32* vtable;
    int spellIndex;
    char* spellName;

    window = EQ_GLOBAL(void*, 0x006018C0);
    if (window != NULL && *(s32*)((u8*)window + 0x10) != 0)
    {
        vtable = *(u32**)window;
        ((WindowAction_guess)vtable[2])(window);
    }

    window = EQ_GLOBAL(void*, 0x00604984);
    if (window != NULL && *(s32*)((u8*)window + 0x10) != 0)
    {
        *(u32*)((u8*)window + 0x1E8) = 1;
        window = EQ_GLOBAL(void*, 0x00604984);
        vtable = *(u32**)window;
        ((WindowAction_guess)vtable[2])(window);
    }

    prompt = EQ_GLOBAL(void*, 0x006E4E20);
    if (prompt == NULL || *(s32*)((u8*)prompt + 0x10) != 0)
        return;

    memcpy((u8*)prompt + 0xA8, message, 0xA0);
    if (_strcmpi((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0xDC, EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->name) != 0)
        return;

    prompt = EQ_GLOBAL(void*, 0x006E4E20);
    spellIndex = (int)*(short*)((u8*)message + 0x78);
    *(u8*)((u8*)prompt + 0x17C) = 2;
    window = EQ_GLOBAL(void*, 0x006E4E20);
    vtable = *(u32**)window;
    ((WindowAction_guess)vtable[1])(window);

    if (spellIndex < 0 || spellIndex >= 3000 ||
        (spellName = EQ_ARRAY(char*, 0x005FE9A0)[spellIndex]) == NULL)
    {
        sprintf((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x1B9, (const char*)0x0055E9D4,
             EQ_METHOD(1, char*, 0x004D02FD, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))((char*)message + 0x58));
    }
    else
    {
        sprintf((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x1B9, (const char*)0x0055E9A4,
             EQ_METHOD(1, char*, 0x004D02FD, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))((char*)message + 0x58),
             spellName);
    }

    display = g_display_guess;
    *(u32*)((u8*)EQ_GLOBAL(void*, 0x006E4E20) + 0x178) = *(u32*)((u8*)display + 0xBC) + 3600000u;
}

// FUNCTION: TRILOGY 0x00495D83
// Finishes the resurrection flow for the local character.
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
// __cdecl, resurrect-complete message (flag at +0x9C, percent at +0x30). Clears the buffs, restores mana and
// hit points (a resurrection under 100% leaves a third of the hit points, no mana and a sickness buff), puts the
// local player at the corpse position and floor, or zones to the other zone.
void msg_resurrect_complete(void* message)
{
    int i;
    float floorZ;

    if (*(s32*)((u8*)message + 0x9C) == 1)
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))((const char*)0x0055EA44, 0x108);
        for (i = 0; i < 15; i++)
        {
            g_pc_guess->buffs[i].visible = 0;
            g_pc_guess->buffs[i].bard_modifier = 0;
            g_pc_guess->buffs[i].spell_id = -1;
            g_pc_guess->buffs[i].level = 0;
            *(u16*)((u8*)&g_pc_guess->buffs[i].duration) = 0;
            *(u16*)((u8*)&g_pc_guess->buffs[i].duration + 2) = 0;
        }
        if (*(s32*)((u8*)message + 0x30) < 100)
        {
            g_pc_guess->mana = 0;
            g_pc_guess->cur_hp = (s16)(EQ_Character_Max_HP_guess(g_pc_guess, 0) / 3);
            if (g_pc_guess->race == 10 || g_pc_guess->race == 9 || g_pc_guess->race == 2 ||
                g_pc_guess->race == 8)
                g_pc_guess->buffs[0].spell_id = 0x2F5;
            else
                g_pc_guess->buffs[0].spell_id = 0x2F4;
            *(u16*)((u8*)&g_pc_guess->buffs[0].duration) = 0x28;
            *(u16*)((u8*)&g_pc_guess->buffs[0].duration + 2) = 0;
            g_pc_guess->buffs[0].visible = 2;
            g_pc_guess->buffs[0].bard_modifier = 0;
            g_pc_guess->buffs[0].level = 0x34;
        }
        else
        {
            g_pc_guess->mana = (s16)EQ_METHOD(0, s32, 0x00419BCA, (CharacterProfile*), g_pc_guess)();
            g_pc_guess->cur_hp = (s16)EQ_Character_Max_HP_guess(g_pc_guess, 0);
        }
        pinstEQ_Main_guess->uiDirty_u32_guess = 1;
        *(u32*)((u8*)g_local_player_guess->actor_info + 0x18C) = 0;
        g_pc_guess->boat[0] = '\0';
        g_local_player_guess->y = pinstEQ_Requestor_guess->resurrectionMessage_guess.y;
        g_local_player_guess->x = pinstEQ_Requestor_guess->resurrectionMessage_guess.x;
        g_local_player_guess->z = pinstEQ_Requestor_guess->resurrectionMessage_guess.z;
        g_local_player_guess->dy = 0.0f;
        g_local_player_guess->dx = 0.0f;
        g_local_player_guess->dz = 0.0f;
        g_local_player_guess->delta_heading = 0.0f;
        g_local_player_guess->unk_0x5C = 0.0f;
        g_local_player_guess->speed = 0.0f;
        if (strcmp((const char*)(EQ_GLOBAL(u8*, 0x006E4E20) + 0xAC), g_local_player_guess->zone) == 0)
        {
            floorZ = CDisplay_GetPlayerFloorHeight_guess(g_display_guess, g_local_player_guess,
                         g_local_player_guess->y, g_local_player_guess->x,
                         g_local_player_guess->z + 1.0f, 1);
            if (floorZ != -1e27f)
            {
                g_local_player_guess->actor_info->floor_z = floorZ;
                g_local_player_guess->z = floorZ + g_local_player_guess->height_offset;
            }
            CDisplay_UpdatePlayerActor_guess(g_display_guess, g_local_player_guess);
        }
        else
        {
            EQ_METHOD(3, void, 0x004DC1B5, (CEverQuest*, const char*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(
                 (const char*)(EQ_GLOBAL(u8*, 0x006E4E20) + 0xAC), (const char*)0x0055EA30, 0);
        }
    }
    else
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))((const char*)0x0055EA04, 0x108);
    }
}

// FUNCTION: TRILOGY 0x004960ae
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name UpdateSpellBookSettings, low, does not fit)
// __cdecl. Copies platinum, gold, silver and copper (four dwords) into the profile, when there is one.
void SetPlayerMoney_guess(s32* coins)
{
    if (g_pc_guess != NULL)
    {
        g_pc_guess->platinum = coins[0];
        g_pc_guess->gold = coins[1];
        g_pc_guess->silver = coins[2];
        g_pc_guess->copper = coins[3];
    }
}

// FUNCTION: TRILOGY 0x004960f2
// Clears the matching corpse item slot after recording the loot response.
/* fake match: the slot address is written out at the load and at the store; a local for it is copy-propagated into the load. */
void HandleCorpseLootResponse_guess(int message)
{
    int slotItem;
    int* countdown;
    u32 timestamp;
    int slot;
    short* idEntry;
    short itemId;

    if (EQ_GLOBAL(int, 0x006E4E44) != 0)
    {
        if (EQ_GLOBAL(void*, 0x006018C0) != NULL &&
            *((int*)EQ_GLOBAL(void*, 0x006018C0) + 4) != 0)
        {
            ((void (__fastcall *)(void*))(*(u32*)(*(u32*)EQ_GLOBAL(void*, 0x006018C0) + 8)))
                (EQ_GLOBAL(void*, 0x006018C0));
        }
        if (EQ_GLOBAL(void*, 0x00604984) != NULL &&
            *((int*)EQ_GLOBAL(void*, 0x00604984) + 4) != 0)
        {
            *((int*)EQ_GLOBAL(void*, 0x00604984) + 0x7A) = 1;
            ((void (__fastcall *)(void*))(*(u32*)(*(u32*)EQ_GLOBAL(void*, 0x00604984) + 8)))
                (EQ_GLOBAL(void*, 0x00604984));
        }
        countdown = (int*)(EQ_GLOBAL(int, 0x006E4E44) + 0x94);
        if (*countdown > 0)
            *countdown = *countdown - 1;
        else
            *countdown = 0;

        EQ_GLOBAL(int, 0x006B6588) = EQ_GLOBAL(int, 0x006B6588) + 1;
        if (9 < EQ_GLOBAL(int, 0x006B6588))
            EQ_GLOBAL(int, 0x006B6588) = 0;
        EQ_ARRAY(int, 0x006B6560)[EQ_GLOBAL(int, 0x006B6588)] = 8;
        timestamp = (u32)time(NULL);
        EQ_ARRAY(u32, 0x006B6538)[EQ_GLOBAL(int, 0x006B6588)] = timestamp;
        itemId = *(short*)(message + 8);
        if (itemId < 0)
        {
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(
                EQ_ARRAY(char, 0x0055EA64), 0x111);
            pinstEQ_Main_guess->lootReady_guess = 1;
        }
        else
        {
            idEntry = (short*)(EQ_GLOBAL(int, 0x006E4E44) + 0x6C8B6);
            for (slot = 0; slot < 0x1E; slot++, idEntry++)
            {
                if (itemId == *idEntry)
                {
                    if ((short)slot >= 0)
                    {
                        slotItem = *(int*)(EQ_GLOBAL(int, 0x006E4E44) + ((short)slot * 4 + 0x6C5E0));
                        if (slotItem != 0)
                        {
                            operator delete( (void*)slotItem);
                            *(int*)(EQ_GLOBAL(int, 0x006E4E44) + ((short)slot * 4 + 0x6C5E0)) = 0;
                        }
                    }
                    break;
                }
            }
            pinstEQ_Main_guess->lootReady_guess = 1;
            pinstEQ_Main_guess->uiDirty_guess = 1;
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004966b6
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
// Reports the local player's consent status after filtering ignored senders.
void msg_consent_verify(int message)
{
    const u8* packet;
    const char* packetName;
    char text[512];
    int nameComparison;

    if (EQ_GLOBAL(void*, 0x006E4DA8) == NULL)
        return;

    packet = (const u8*)(u32)message;
    nameComparison = _strcmpi(
        (const char*)EQ_GLOBAL(EQPlayer*, 0x006E4D8C) + 1, packetName = (const char*)packet + 0x20);
    if (nameComparison == 0)
    {
        if (packet[0x40] != 0)
            sprintf(text, EQ_ARRAY(char, 0x0055EBA0), packet);
        else
            sprintf(text, EQ_ARRAY(char, 0x0055EB6C), packet);
    }
    else
    {
        if (EQ_METHOD(1, int, 0x004CFF2D, (void*, char*), g_everquest_guess)((char*)packetName) != 0)
            return;
        if (packet[0x40] != 0)
            sprintf(text, EQ_ARRAY(char, 0x0055EB38), packetName);
        else
            sprintf(text, EQ_ARRAY(char, 0x0055EB00), packetName);
    }
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(text, 0x111);
}

// The old UI's windows as HandleWorldMessage drives them: vtable slot 1 opens, slot 2 closes, +0x10 = open.
class HWMWindow_guess
{
public:
    virtual void Slot0_guess(void);
    virtual void Activate_guess(void);
    virtual void Deactivate_guess(void);
    virtual void Slot3_guess(void);
    virtual void Show_guess(int mode);

    u8      unknown0x0004[0xC]; /* 0x0004  */
    int     isOpen_guess;       /* 0x0010* */
    u8      unknown0x0014[0x15C]; /* 0x0014  */
    int     guildRequestValue_guess; /* 0x0170* */
    u8*     item_guess;         /* 0x0174* short views keep their casts (no union in this TU) */
    int     requestDeadline_guess; /* 0x0178* */
    u8      requestMode_guess;  /* 0x017C* */
    u8      unknown0x017D[0x6B]; /* 0x017D  */
    int     forcedClose_guess;  /* 0x01E8* */
    u8      unknown0x01EC[0x35C0]; /* 0x01EC  */
    EQ_Item* inspectedItem_guess; /* 0x37AC* */
};
C_ASSERT(offsetof(HWMWindow_guess, isOpen_guess) == 0x0010);
C_ASSERT(offsetof(HWMWindow_guess, guildRequestValue_guess) == 0x0170);
C_ASSERT(offsetof(HWMWindow_guess, item_guess) == 0x0174);
C_ASSERT(offsetof(HWMWindow_guess, requestDeadline_guess) == 0x0178);
C_ASSERT(offsetof(HWMWindow_guess, requestMode_guess) == 0x017C);
C_ASSERT(offsetof(HWMWindow_guess, forcedClose_guess) == 0x01E8);
C_ASSERT(offsetof(HWMWindow_guess, inspectedItem_guess) == 0x37AC);

struct CEverQuest_HandleWorldMessage_type_guess
{
    bool HandleWorldMessage(void* connection, u32 opcode, u8* data, u32 size);
};

typedef int (WINAPI *HWMMessageBoxFunction_guess)(HWND, LPCSTR, LPCSTR, UINT);
typedef BOOL (WINAPI *HWMDestroyWindowFunction_guess)(HWND);
typedef void (WINAPI *HWMCriticalSectionFunction_guess)(LPCRITICAL_SECTION);
typedef void (WINAPI *HWMSleepFunction_guess)(DWORD);

#define HWM_EQMAIN                  EQ_GLOBAL(u8*, 0x006E4E44)
#define HWM_PLAYER_TOP              EQ_GLOBAL(EQPlayer*, 0x006E4D58)
#define HWM_WINDOW(va)              EQ_GLOBAL(HWMWindow_guess*, va)
#define HWM_IS_OPEN(window)         (*(int*)((u8*)(window) + 0x10))
#define HWM_FIELD(type, object, offset) (*(type*)((u8*)(object) + (offset)))
#define HWM_DSP_CHAT(text, color)   EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(text, color)
#define HWM_TRIM_NAME(name)         EQ_METHOD(1, char*, 0x004D02FD, (void*, const char*), g_everquest_guess)(name)
#define HWM_STRIP_SHIP(name)        EQ_METHOD(1, char*, 0x004D024E, (void*, const char*), this)(name)
#define g_profile_stream_guess      EQ_ARRAY(ZStream_guess, 0x00596338)
#define g_profile_buffer_guess      EQ_ARRAY(MsgPlayerProfile, 0x005922F4)
#define HWM_STRIP_NAME(name)       EQ_METHOD(1, char*, 0x004D0436, (void*, const char*), g_everquest_guess)(name)
#define HWM_GET_PLAYER_BY_ID(id)   EQ_METHOD(1, EQPlayer*, 0x004B1416, (EQPlayer*, int), HWM_PLAYER_TOP)(id)
#define HWM_GET_PLAYER(name)        EQ_METHOD(1, EQPlayer*, 0x004B1435, (EQPlayer*, const char*), HWM_PLAYER_TOP)(name)
#define HWM_SEND(opcode, msg, size, reliable) \
    send_message_guess(g_world_connection_guess, opcode, msg, size, reliable)
#define HWM_WAVE_PLAY(sound)        EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), g_sound_manager_ptr_guess)(sound, 0)
#define HWM_FLOOR_HEIGHT(player, y, x, z) \
    EQ_METHOD(5, float, 0x0041436D, (void*, EQPlayer*, float, float, float, int), g_display_guess)(player, y, x, z, 1)
#define HWM_FLOOR_HEIGHT_AT(y, x, z) \
    EQ_METHOD(3, float, 0x00414245, (void*, float, float, float), g_display_guess)(y, x, z)
#define HWM_CREATE_ACTOR(name, y, x, z, heading) \
    EQ_METHOD(9, int, 0x004112DF, (void*, char*, float, float, float, float, float, float, int, int), g_display_guess)( \
        name, y, x, z, heading, 0.0f, 0.0f, 1, 1)
#define HWM_UPDATE_ACTOR(player)   EQ_METHOD(1, void, 0x00410ABF, (void*, EQPlayer*), g_display_guess)(player)
#define HWM_LOG_LOOT_EVENT(code) \
    do \
    { \
        if (++EQ_GLOBAL(int, 0x006B6588) > 9) \
            EQ_GLOBAL(int, 0x006B6588) = 0; \
        EQ_ARRAY(int, 0x006B6560)[EQ_GLOBAL(int, 0x006B6588)] = (code); \
        EQ_ARRAY(int, 0x006B6538)[EQ_GLOBAL(int, 0x006B6588)] = (int)time(NULL); \
    } while (0)
#define HWM_COUNT_DOWN(field) \
    do \
    { \
        if ((field) > 0) \
            (field)--; \
        else \
            (field) = 0; \
    } while (0)
#define HWM_CLOSE_WORLD() \
    EQ_CDECL(void, 0x004F21D0, (void*, int, int, int))(EQ_GLOBAL(void*, 0x006B3194), 10000, 0, 0)

#define CEverQuest CEverQuest_HandleWorldMessage_type_guess
class EQObjectAllocation_guess
{
public:
    u8 bytes[0x58];
    // EXTERNAL: TRILOGY 0x004B00AF EQObjectAllocation_guess::EQObjectAllocation_guess
    EQObjectAllocation_guess(u8* msg, int zero, u8* name, u8* actor);
};

class EQSwitchAllocation_guess
{
public:
    u8 bytes[0x90];
    // EXTERNAL: TRILOGY 0x004BD55F EQSwitchAllocation_guess::EQSwitchAllocation_guess
    EQSwitchAllocation_guess(int first, int second, u8* msg, int fourth);
};

class EQPlayerAllocation_guess : public EQPlayer
{
public:
    // EXTERNAL: TRILOGY 0x004B03EB EQPlayerAllocation_guess::EQPlayerAllocation_guess
    EQPlayerAllocation_guess(EQPlayer* image, int first, int second, int third, int fourth);
};

class EquipmentAllocation_guess
{
public:
    u8 bytes[0x124];
    // EXTERNAL: TRILOGY 0x00436099 EquipmentAllocation_guess::EquipmentAllocation_guess
    EquipmentAllocation_guess();
};

class ContainerAllocation_guess
{
public:
    u8 bytes[0xDC];
    // EXTERNAL: TRILOGY 0x00434040 ContainerAllocation_guess::ContainerAllocation_guess
    ContainerAllocation_guess();
};

class NoteAllocation_guess
{
public:
    u8 bytes[0xD0];
    // EXTERNAL: TRILOGY 0x0044FF75 NoteAllocation_guess::NoteAllocation_guess
    NoteAllocation_guess();
};

class MoneyListAllocation_guess
{
public:
    u8 bytes[0x70];
    // EXTERNAL: TRILOGY 0x004AD938 MoneyListAllocation_guess::MoneyListAllocation_guess
    MoneyListAllocation_guess();
};

class ItemListAllocation_guess
{
public:
    u8 bytes[0xF0];
    // EXTERNAL: TRILOGY 0x004A02E5 ItemListAllocation_guess::ItemListAllocation_guess
    ItemListAllocation_guess();
};

class MissileAllocation_guess
{
public:
    u8 bytes[0x74];
    // EXTERNAL: TRILOGY 0x004AC67C MissileAllocation_guess::MissileAllocation_guess
    MissileAllocation_guess(int first, int second, u8* msg, int fourth, int fifth, int sixth);
};

class BBoardAllocation_guess
{
public:
    u8 bytes[0x5C];
    // EXTERNAL: TRILOGY 0x0049062D BBoardAllocation_guess::BBoardAllocation_guess
    BBoardAllocation_guess(int unused);
};

// FUNCTION: TRILOGY 0x00496743
// Handles one message received from the world server; arguments are the connection, opcode, data and size.
// One large switch on opcode (about 220 cases, in the 0x2xxx range) copies each payload into game state and calls
// the handler for it; many per-message scratch buffers sit on the stack.
// Returns true when the switch finishes; the allocation_failed exit releases the critical section at 0x006048e8,
// sets *this to 1 and returns false.
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
bool CEverQuest::HandleWorldMessage(void* connection, u32 opcode, u8* data, u32 size)
{
    EQPlayer* player;
    EQ_Item* newItem;
    u8* object;
    u8* item;
    u32 parsePos;
    int parseDone;
    int messageSize;
    u8 itemMessage[0x170];
    u8 coinMessage[0x44];
    MsgClientError errorMessage;
    MsgBeginCast beginCast;
    MsgZoneChange zoneChange;
    u32 duelMessage[2];
    short* bagItems;
    char echoText[0x800];
    u8 guildReply[0x48];
    u8 deleteMessage[0x24];
    u8 coinReply[0x28];
    s32* coins;
    u32 filter;
    u32 decompressed;
    int connectResult;
    u8 zoneServer[0x84];
    MsgMoneyTrade moneyTrade;
    u8 summonReply[0x2C];
    u32 savedLink64;
    EQPlayer* savedNext;
    EQActorInfo_guess* savedActor;
    void* savedCharacter;
    EQPlayer* savedTop;
    u32 savedLink0;
    u32 savedLink4;
    EQ_Item* receivedItem;
    MsgCastOn castOn;
    MsgDeath death;
    u8 soundMessage[8];
    EQPlayer* killer;
    u32 updateCount;
    int animation[3];
    u8 tradeReply[0xC];
    u8* stackCount;
    u8 busyTell[0x118];
    MsgMoveCoin coinMove;
    char fileName[256];
    char specialLine[0x200];
    char chatText[0x800];
    char languageText[64];
    char babelText[0x800];
    u8 relayMessage[0x880];
    u8 afkReply[0x200];
    u8 inspectAnswer[0x414];
    char firstWord[256];
    char line[0x800];
    HWMWindow_guess* window;
    int slot;
    int index;
    int found;
    int color;
    int guildRequest;
    float floor;
    u16 savedRace;
    short savedTexture;
    short savedHelm;
    short savedFace;
    u8 savedGender;
    short affectSlot;
    MsgTimeOfDay timeOfDay;
    char text[512];
    char fromText[256];
    char actorTag[64];
    char nearestTag[64];

    color = 10;
    switch (opcode)
    {
    case 0x2025:
        EQ_CDECL(void, 0x004E87FD, (u8*))(data);
        break;
    case 0x2022:
        HWM_DSP_CHAT(EQ_ARRAY(char, 0x005600FC), 0x108);
        break;
    case 0x2021:
        EQ_CDECL(void, 0x00491B5D, (u8*))(data);
        break;
    case 0x200F:
        EQ_CDECL(void, 0x00491BF0, (u8*))(data);
        break;
    case 0x20D4:
        player = HWM_GET_PLAYER((const char*)data);
        if (player == NULL)
            break;
        if (*(short*)(data + 0x5C) == 0)
            break;
        if (HWM_EQMAIN != NULL)
            HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        if (player->spawn_id != g_local_player_guess->spawn_id)
            break;
        strcpy(g_pc_guess->unknown0x0A78, (const char*)data + 0x3C);
        break;
    case 0x2117:
        EQ_CDECL(void, 0x00491272, (void*, u32, u8*, u32))(connection, opcode, data, size);
        break;
    case 0x211E:
        if (HWM_EQMAIN == NULL)
            break;
        if (strcmp(g_pc_guess->name, (const char*)data) != 0)
            break;
        switch (*(int*)(data + 0x3C))
        {
        case 0:
            sprintf(text, EQ_ARRAY(char, 0x0056009C), data + 0x1E);
            break;
        case 1:
            sprintf(text, EQ_ARRAY(char, 0x005600BC), data + 0x1E);
            break;
        case 2:
            sprintf(text, EQ_ARRAY(char, 0x005600DC), data + 0x1E);
            break;
        default:
            goto world_done;
        }
        HWM_DSP_CHAT(text, 0x111);
        break;
    case 0x20CF:
        EQ_CDECL(void, 0x00496598, (u8*))(data);
        break;
    case 0x20D0:
        player = HWM_GET_PLAYER_BY_ID(*(int*)(data + 4));
        if (player == NULL)
            break;
        if (player->spawn_id != EQ_GLOBAL(u32, 0x006B64C8))
            break;
        if (data[8] == 0)
            sprintf(text, EQ_ARRAY(char, 0x00560064), player->name);
        else if (data[8] == 1)
            sprintf(text, EQ_ARRAY(char, 0x00560030), player->name);
        else if (data[8] == 2)
            sprintf(text, EQ_ARRAY(char, 0x00560004), player->name);
        else
            goto duel_reset;
        HWM_DSP_CHAT(text, 0xF);
    duel_reset:
        EQ_GLOBAL(u8, 0x006B64D0) = 0;
        EQ_GLOBAL(u32, 0x006B64C8) = 0;
        break;
    case 0x20D9:
        DbgLog_guess(EQ_ARRAY(char, 0x0055FFCC));
        if (EQ_GLOBAL(void*, 0x006B3194) != NULL)
        {
            HWM_CLOSE_WORLD();
            EQ_GLOBAL(void*, 0x006B3194) = NULL;
        }
        EQ_GLOBAL(HWMMessageBoxFunction_guess, 0x00534230)(HWM_FIELD(HWND, this, 0x22894),
            EQ_ARRAY(char, 0x0055FFBC), EQ_ARRAY(char, 0x0055FFC4), 0);
        EQ_GLOBAL(HWMDestroyWindowFunction_guess, 0x00534234)(HWM_FIELD(HWND, this, 0x22894));
        break;
    case 0x208C:
        object = EQ_GLOBAL(u8*, 0x006E4DF4);
        if (object == NULL)
            break;
        if (g_local_player_guess == NULL)
            break;
        if (g_local_player_guess->gm == 0)
            break;
        if (HWM_FIELD(int, g_everquest_guess, 4) != 5)
            break;
        if (HWM_IS_OPEN(object) != 0)
        {
            slot = HWM_FIELD(int, object, 0x8C);
            if (slot == 3 || slot == 4)
                break;
        }
        strcpy((char*)object + 0xB4, (const char*)data);
        strcpy(EQ_GLOBAL(char*, 0x006E4DF4) + 0xD2, (const char*)data + 0x1E);
        HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4DF4), 0xF0) = *(int*)(data + 0x3C);
        HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4DF4), 0xF4) = *(int*)(data + 0x40);
        strcpy(EQ_GLOBAL(char*, 0x006E4DF4) + 0xF8, (const char*)data + 0x44);
        EQ_GLOBAL(u8*, 0x006E4DF4)[0x116] = 0;
        HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4DF4), 0x80) = EQ_CDECL(int, 0x004A461E, (void))();
        object = EQ_GLOBAL(u8*, 0x006E4DF4);
        if (HWM_IS_OPEN(object) != 0)
        {
            HWM_FIELD(int, object, 0x88) = HWM_FIELD(int, object, 0x8C);
            HWM_WINDOW(0x006E4DF4)->Deactivate_guess();
        }
        else
        {
            HWM_FIELD(int, object, 0x88) = -1;
        }
        HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4DF4), 0x8C) = 3;
        HWM_WINDOW(0x006E4DF4)->Activate_guess();
        break;
    case 0x208B:
        if (EQ_GLOBAL(void*, 0x006E4E70) == NULL)
            break;
        if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E70))() == 0)
            break;
        if (data[0] != 0)
        {
            HWM_FIELD(short, EQ_GLOBAL(u8*, 0x006E4E70), 0x1BF0) = 6;
        }
        else
        {
            HWM_FIELD(short, EQ_GLOBAL(u8*, 0x006E4E70), 0x1BF0) = 7;
            HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4E70), 0x1BB4) = EQ_CDECL(int, 0x004A461E, (void))();
        }
        HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4E70), 0x664) = 0;
        break;
    case 0x2080:
        if (HWM_EQMAIN != NULL)
            HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        player = HWM_GET_PLAYER_BY_ID(*(int*)data);
        if (player != NULL)
            sprintf(fromText, EQ_ARRAY(char, 0x0055FFB0), HWM_TRIM_NAME(player->name));
        else
            fromText[0] = 0;
        EQ_METHOD(1, void, 0x0040240F, (void*, int), g_pc_guess)(*(int*)(data + 8));
        if (*(int*)(data + 0x14) > 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055FF94), *(int*)(data + 0x14), fromText);
            HWM_DSP_CHAT(text, 0x111);
            g_pc_guess->copper += *(int*)(data + 0x14);
        }
        if (*(int*)(data + 0x18) > 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055FF78), *(int*)(data + 0x18), fromText);
            HWM_DSP_CHAT(text, 0x111);
            g_pc_guess->silver += *(int*)(data + 0x18);
        }
        if (*(int*)(data + 0x1C) > 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055FF60), *(int*)(data + 0x1C), fromText);
            HWM_DSP_CHAT(text, 0x111);
            g_pc_guess->gold += *(int*)(data + 0x1C);
        }
        if (*(int*)(data + 0x20) > 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055FF44), *(int*)(data + 0x20), fromText);
            HWM_DSP_CHAT(text, 0x111);
            g_pc_guess->platinum += *(int*)(data + 0x20);
        }
        if (g_sound_manager_ptr_guess == NULL)
            break;
        HWM_WAVE_PLAY(0x8D);
        break;
    case 0x207D:
        if (g_pc_guess == NULL)
            break;
        if (g_local_player_guess == NULL)
            break;
        if (strcmp((const char*)data, g_local_player_guess->name) != 0)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        if (HWM_IS_OPEN(HWM_WINDOW(0x006E4E5C)) != 0)
        {
            HWM_FIELD(u8*, HWM_WINDOW(0x006E4E5C), 0x174)[0xD6] = 0;
            HWM_WINDOW(0x006E4E5C)->Deactivate_guess();
            HWM_FIELD(u8*, HWM_WINDOW(0x006E4E5C), 0x174) = NULL;
        }
        if (HWM_IS_OPEN(HWM_WINDOW(0x006E4E58)) != 0)
        {
            HWM_FIELD(u8*, HWM_WINDOW(0x006E4E58), 0x174)[0xD6] = 0;
            HWM_WINDOW(0x006E4E58)->Deactivate_guess();
            HWM_FIELD(u8*, HWM_WINDOW(0x006E4E58), 0x174) = NULL;
        }
        window = HWM_WINDOW(0x006E4E60);
        if (window->isOpen_guess != 0)
        {
            window->Deactivate_guess();
            HWM_FIELD(int, HWM_WINDOW(0x006E4E60), 0x37AC) = 0;
        }
        EQ_METHOD(0, void, 0x004D56B0, (void*), g_everquest_guess)();
        for (slot = 0; slot < 30; slot++)
        {
            if (g_pc_guess->inventory_items[slot] != NULL)
            {
                operator delete(g_pc_guess->inventory_items[slot]);
                g_pc_guess->inventory_items[slot] = NULL;
            }
        }
        if (HWM_EQMAIN != NULL)
            EQ_CDECL(void, 0x004A204D, (CharacterProfile*))(g_pc_guess);
        for (slot = 0; slot < 8; slot++)
        {
            if (g_pc_guess->bank_items[slot] != NULL)
            {
                operator delete(g_pc_guess->bank_items[slot]);
                g_pc_guess->bank_items[slot] = NULL;
            }
        }
        g_pc_guess->gold = 0;
        g_pc_guess->platinum = 0;
        g_pc_guess->silver = 0;
        g_pc_guess->copper = 0;
        g_pc_guess->gold_bank = 0;
        g_pc_guess->copper_bank = 0;
        g_pc_guess->silver_bank = 0;
        g_pc_guess->platinum_bank = 0;
        g_displayManager_guess[0x3C] = 0;
        g_displayManager_guess[0x3D] = 0;
        g_pc_guess->gold_cursor = 0;
        g_pc_guess->platinum_cursor = 0;
        g_pc_guess->silver_cursor = 0;
        g_pc_guess->copper_cursor = 0;
        EQ_GLOBAL(u32, 0x006B7120) = 0x10;
        HWM_SEND(0x21AD, NULL, 0, 1);
        break;
    case 0x2069:
        if (data[0x3C] != 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055FF14), data + 0x3D, *(int*)(data + 0x5C), *(int*)(data + 0x60),
                *(int*)(data + 0x64));
            HWM_DSP_CHAT(text, 0xA);
            break;
        }
    player_not_found:
        sprintf(text, EQ_ARRAY(char, 0x0055FEEC));
        HWM_DSP_CHAT(text, 0xA);
        break;
    case 0x206E:
        if (g_local_player_guess == NULL)
            goto player_not_found;
        if (g_control_player_guess != g_local_player_guess)
            break;
        if ((s8)g_pc_guess->unknown0x0076 > 0)
            break;
        if (g_control_player_guess->stand_state == 0x66)
            break;
        HWM_FIELD(int, g_local_player_guess->actor_info, 0x18C) = 0;
        g_pc_guess->boat[0] = 0;
        g_local_player_guess->y = (float)*(int*)(data + 0x5C);
        g_local_player_guess->x = (float)*(int*)(data + 0x60);
        g_local_player_guess->z = (float)*(int*)(data + 0x64);
        g_local_player_guess->dy = 0.0f;
        g_local_player_guess->dx = 0.0f;
        g_local_player_guess->dz = 0.0f;
        g_local_player_guess->delta_heading = 0.0f;
        g_local_player_guess->unk_0x5C = 0.0f;
        g_local_player_guess->speed = 0.0f;
        if (strcmp((const char*)data + 0x3D, g_pc_guess->zone) == 0)
        {
            floor = HWM_FLOOR_HEIGHT(g_local_player_guess, (float)*(int*)(data + 0x5C), (float)*(int*)(data + 0x60),
                (float)*(int*)(data + 0x64) + EQ_GLOBAL(float, 0x005344A4));
            if (floor != EQ_GLOBAL(float, 0x005351F4))
            {
                HWM_FIELD(float, g_local_player_guess->actor_info, 0x48) = floor;
                g_local_player_guess->z = floor + g_local_player_guess->height_offset;
            }
            HWM_UPDATE_ACTOR(g_local_player_guess);
            break;
        }
        EQ_METHOD(3, void, 0x004DC1B5, (void*, const char*, const char*, int), this)(
            (const char*)data + 0x3D, EQ_ARRAY(char, 0x0055FEE0), 0);
        break;
    case 0x206A:
        if (strcmp((const char*)data, g_local_player_guess->name) != 0)
            break;
        g_local_player_guess->stand_state = 0x66;
        g_pc_guess->unknown0x0076 = 2;
        g_local_player_guess->speed = 0.0f;
        g_local_player_guess->delta_heading = 0.0f;
        break;
    case 0x20BE:
        if (strcmp((const char*)data, g_local_player_guess->name) != 0)
            break;
        g_local_player_guess->stand_state = 0x66;
        g_pc_guess->unknown0x0076 = 2;
        g_local_player_guess->speed = 0.0f;
        g_local_player_guess->delta_heading = 0.0f;
        EQ_GLOBAL(u32, 0x006E2054) = 1;
        break;
    case 0x206B:
    case 0x20C1:
        if (strcmp((const char*)data, g_local_player_guess->name) != 0)
            break;
        if (EQ_METHOD(1, int, 0x004199C0, (void*, int), g_pc_guess)(0) > 0)
            g_pc_guess->unknown0x0076 = 0;
        g_local_player_guess->stand_state = 0x64;
        EQ_GLOBAL(u32, 0x006E2054) = 0;
        break;
    case 0x206C:
        if (_strcmpi(g_local_player_guess->name, (const char*)data) != 0)
            break;
        sprintf(text, EQ_ARRAY(char, 0x0055FEB8), HWM_TRIM_NAME((const char*)data));
        HWM_DSP_CHAT(text, 0xA);
        if (g_sound_manager_ptr_guess != NULL)
            HWM_WAVE_PLAY(0x8F);
        g_pc_guess->cur_hp = -10000;
        g_local_player_guess->cur_hp = -10000;
        HWM_FIELD(int, g_local_player_guess->actor_info, 0x68) = 0;
        break;
    case 0x206D:
        if (_strcmpi(g_local_player_guess->name, (const char*)data) != 0)
            break;
        HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055FE90), 0xA);
    kicked_from_world:
        EQ_GLOBAL(u8, 0x006B7111) = 1;
        HWM_FIELD(int, this, 0x223F4) = 1;
        HWM_FIELD(int, this, 4) = 0xFF;
        EQ_GLOBAL(u32, 0x006B63F4) = 0xCB;
        HWM_FIELD(int, this, 0) = 1;
        if (EQ_GLOBAL(void*, 0x006B3194) == NULL)
            break;
        HWM_CLOSE_WORLD();
        EQ_GLOBAL(void*, 0x006B3194) = NULL;
        break;
    case 0x20C5:
        if (g_local_player_guess == NULL)
            break;
        HWM_FIELD(int, g_local_player_guess->actor_info, 0x18C) = 0;
        g_pc_guess->boat[0] = 0;
        g_local_player_guess->y = (float)*(int*)(data + 0x5C);
        g_local_player_guess->x = (float)*(int*)(data + 0x60);
        g_local_player_guess->z = (float)*(int*)(data + 0x64);
        if (_strcmpi((const char*)data + 0x3D, g_pc_guess->zone) == 0)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055FE78), 0xF);
            floor = HWM_FLOOR_HEIGHT(g_local_player_guess, (float)*(int*)(data + 0x5C), (float)*(int*)(data + 0x60),
                (float)*(int*)(data + 0x64) + EQ_GLOBAL(float, 0x005344A4));
            if (floor != EQ_GLOBAL(float, 0x005351F4))
            {
                HWM_FIELD(float, g_local_player_guess->actor_info, 0x48) = floor;
                g_local_player_guess->z = floor + g_local_player_guess->height_offset;
            }
            g_local_player_guess->dy = 0.0f;
            g_local_player_guess->dx = 0.0f;
            g_local_player_guess->dz = 0.0f;
            g_local_player_guess->delta_heading = 0.0f;
            g_local_player_guess->unk_0x5C = 0.0f;
            g_local_player_guess->speed = 0.0f;
            HWM_UPDATE_ACTOR(g_local_player_guess);
            break;
        }
        HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055FE54), 0xF);
        EQ_METHOD(3, void, 0x004DC1B5, (void*, const char*, const char*, int), this)(
            (const char*)data + 0x3D, EQ_ARRAY(char, 0x0055FE44), 0);
        break;
    case 0x2078:
        if (strcmp((const char*)data, g_local_player_guess->name) != 0)
            break;
        EQ_GLOBAL(u8, 0x006B6464) = 1;
        break;
    case 0x20C9:
        if (strcmp((const char*)data, g_local_player_guess->name) != 0)
            break;
        EQ_GLOBAL(u8, 0x006B6464) = 0;
        break;
    case 0x2079:
        if (strcmp((const char*)data, g_local_player_guess->name) != 0)
            break;
        EQ_GLOBAL(u8, 0x006B6465) = 1;
        break;
    case 0x20C7:
        if (strcmp((const char*)data, g_local_player_guess->name) != 0)
            break;
        EQ_GLOBAL(u8, 0x006B6465) = 0;
        break;
    case 0x207A:
        if (strcmp((const char*)data, g_local_player_guess->name) != 0)
            break;
        strcpy(EQ_ARRAY(char, 0x006B64A8), (const char*)data + 0x1E);
        break;
    case 0x2102:
        if (strcmp((const char*)data, g_local_player_guess->name) != 0)
            break;
        EQ_ARRAY(char, 0x006B64A8)[0] = 0;
        break;
    case 0x20CB:
        player = HWM_GET_PLAYER((const char*)data);
        if (player == NULL)
            break;
        if (*(short*)(data + 0x5C) == 0)
            break;
        if (HWM_EQMAIN != NULL)
            HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        strcpy(player->name, (const char*)data + 0x3C);
        if (player->spawn_id == g_local_player_guess->spawn_id)
            strcpy(g_pc_guess->name, (const char*)data + 0x3C);
        EQ_METHOD(1, void, 0x00411B7B, (void*, EQPlayer*), g_display_guess)(player);
        break;
    case 0x20B4:
        EQ_CDECL(void, 0x004C1823, (void*, u32, u8*, u32))(connection, opcode, data, size);
        break;
    case 0x20B0:
        EQ_METHOD(1, void, 0x004B45CB, (EQPlayer*, int), g_local_player_guess)(0x64);
        HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055FE20), 4);
        if (HWM_EQMAIN == NULL)
            break;
        HWM_FIELD(int, HWM_EQMAIN, 0x94) = 0;
        HWM_FIELD(int, HWM_EQMAIN, 0xE5C) = 0;
        break;
    case 0x205B:
        memcpy(EQ_ARRAY(MsgNewZone, 0x006B65DC), data, sizeof(MsgNewZone));
        if (EQ_ARRAY(MsgNewZone, 0x006B65DC)->fog_min_clip[0] == EQ_GLOBAL(float, 0x0053510C) &&
            EQ_ARRAY(MsgNewZone, 0x006B65DC)->fog_max_clip[0] == EQ_GLOBAL(float, 0x0053510C))
        {
            EQ_ARRAY(MsgNewZone, 0x006B65DC)->fog_min_clip[0] = 500.0f;
            EQ_ARRAY(MsgNewZone, 0x006B65DC)->fog_max_clip[0] = 600.0f;
            EQ_ARRAY(MsgNewZone, 0x006B65DC)->fog_red[0] = 0xC0;
            EQ_ARRAY(MsgNewZone, 0x006B65DC)->fog_green[0] = 0xC0;
            EQ_ARRAY(MsgNewZone, 0x006B65DC)->fog_blue[0] = 0xC0;
        }
        if (EQ_ARRAY(MsgNewZone, 0x006B65DC)->min_clip < EQ_GLOBAL(float, 0x00535190))
            EQ_ARRAY(MsgNewZone, 0x006B65DC)->min_clip = EQ_GLOBAL(float, 0x00535190);
        if (EQ_ARRAY(MsgNewZone, 0x006B65DC)->max_clip < EQ_GLOBAL(float, 0x00535690))
            EQ_ARRAY(MsgNewZone, 0x006B65DC)->max_clip =
                EQ_ARRAY(MsgNewZone, 0x006B65DC)->fog_max_clip[0] * EQ_GLOBAL(float, 0x005351D4);
        if (EQ_ARRAY(MsgNewZone, 0x006B65DC)->max_clip < EQ_ARRAY(MsgNewZone, 0x006B65DC)->min_clip)
            EQ_ARRAY(MsgNewZone, 0x006B65DC)->max_clip =
                EQ_ARRAY(MsgNewZone, 0x006B65DC)->min_clip * EQ_GLOBAL(float, 0x005351D4);
        HWM_FIELD(int, g_display_guess, 0x14) = EQ_ARRAY(MsgNewZone, 0x006B65DC)->sky - 1;
        EQ_METHOD(0, void, 0x004124DB, (void*), g_display_guess)();
        EQ_GLOBAL(u8, 0x006B6750) = 1;
        break;
    case 0x20F6:
        object = (u8*)new EQObjectAllocation_guess(data, 0, data + 0xC, data + 0x20);
        if (object == NULL)
            break;
        if (HWM_FIELD(int, object, 0x54) != 0)
            break;
        EQ_METHOD(0, void, 0x004B02C9, (void*), object)();
        operator delete(object);
        break;
    case 0x208E:
        object = EQ_METHOD(1, u8*, 0x004BDCC7, (void*, char), EQ_GLOBAL(void*, 0x006E4D78))((char)data[0]);
        if (object == NULL)
            break;
        EQ_METHOD(2, void, 0x004BDF41, (void*, char, int), object)((char)data[1], 0);
        break;
    case 0x209B:
        while (EQ_GLOBAL(u8*, 0x006E4D78) != NULL)
        {
            object = EQ_GLOBAL(u8*, 0x006E4D78);
            EQ_METHOD(0, void, 0x004BDC26, (void*), object)();
            operator delete(object);
        }
        break;
    case 0x2095:
        object = (u8*)new EQSwitchAllocation_guess(0, 0, data, 0);
        if (object == NULL)
            break;
        if (HWM_FIELD(int, object, 0x80) != 0)
            break;
        if (object[0x1C] == 0x35)
            break;
        EQ_METHOD(0, void, 0x004BDC26, (void*), object)();
        operator delete(object);
        break;
    case 0x20F2:
        memcpy(&timeOfDay, data, sizeof(MsgTimeOfDay));
        EQ_GLOBAL(u8*, 0x006E4D54)[0] = timeOfDay.hour;
        EQ_GLOBAL(u8*, 0x006E4D54)[1] = timeOfDay.minute;
        EQ_GLOBAL(u8*, 0x006E4D54)[2] = timeOfDay.day;
        EQ_GLOBAL(u8*, 0x006E4D54)[3] = timeOfDay.month;
        HWM_FIELD(u16, EQ_GLOBAL(u8*, 0x006E4D54), 4) = timeOfDay.year;
        EQ_GLOBAL(u8*, 0x006E4D54)[6] = EQ_GLOBAL(u8*, 0x006E4D54)[0];
        EQ_GLOBAL(u8*, 0x006E4D54)[7] = EQ_GLOBAL(u8*, 0x006E4D54)[1];
        EQ_GLOBAL(u8*, 0x006E4D54)[8] = EQ_GLOBAL(u8*, 0x006E4D54)[2];
        EQ_GLOBAL(u8*, 0x006E4D54)[9] = EQ_GLOBAL(u8*, 0x006E4D54)[3];
        HWM_FIELD(u16, EQ_GLOBAL(u8*, 0x006E4D54), 0xA) = HWM_FIELD(u16, EQ_GLOBAL(u8*, 0x006E4D54), 4);
        HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4D54), 0x10) = (int)time(NULL);
        HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4D54), 0xC) = EQ_CDECL(int, 0x004A461E, (void))();
        EQ_METHOD(0, void, 0x004124DB, (void*), g_display_guess)();
        break;
    case 0x2047:
        EQ_CDECL(void, 0x004923C8, (void*, u32, u8*, u32))(connection, opcode, data, size);
        break;
    case 0x202A:
        if (size != 0x138 || EQ_CDECL(u32, 0x004E04A2, (u8*, int))(data + 4, 0x134) != *(u32*)data)
        {
            DbgLog_guess(EQ_ARRAY(char, 0x0055FDF4));
            HWM_FIELD(int, this, 0x223F4) = 1;
            HWM_FIELD(int, this, 4) = 0xFF;
            EQ_GLOBAL(u32, 0x006B63F4) = 0xCB;
            HWM_FIELD(int, this, 0) = 1;
            if (EQ_GLOBAL(void*, 0x006B3194) == NULL)
                break;
            HWM_CLOSE_WORLD();
            EQ_GLOBAL(void*, 0x006B3194) = NULL;
            break;
        }
        if (HWM_PLAYER_TOP != NULL && HWM_GET_PLAYER_BY_ID(*(int*)(data + 0x84)) != NULL)
            break;
        player = new EQPlayerAllocation_guess((EQPlayer*)(data + 4), 0, 0, 0, 0);
        if (player == NULL)
            goto allocation_failed;
        if (player->type == 0 && player->guild_id != -1)
        {
            if (EQ_CDECL(int, 0x004DFE8D, (int))(player->guild_id) == -1)
            {
                guildRequest = player->guild_id;
                HWM_SEND(0x2128, &guildRequest, 4, 1);
            }
        }
        object = (u8*)player->actor_info;
        if (object != NULL && HWM_FIELD(int, object, 0) != 0)
        {
            if ((u32)(HWM_FIELD(int, g_display_guess, 0xBC) - HWM_FIELD(int, object, 0x19C)) > 12000)
                EQ_METHOD(4, void, 0x004AE653, (void*, EQPlayer*, float, float, float), g_display_guess)(
                    player, player->y, player->x, player->z);
            HWM_UPDATE_ACTOR(player);
            HWM_FIELD(int, player->actor_info, 0x19C) = HWM_FIELD(int, g_display_guess, 0xBC);
        }
        if (player->type == 0 && g_name_buffer_guess[0] != 0)
        {
            for (slot = 0; slot < 5; slot++)
            {
                if (g_group_names_guess[slot * 0x1E] != 0 &&
                    strcmp(&g_group_names_guess[slot * 0x1E], player->name) == 0)
                    g_death_state_guess[slot] = (u32)player;
            }
        }
        if (g_pc_guess == NULL)
            break;
        if (strcmp(g_pc_guess->name, player->name) != 0)
            break;
        g_local_player_guess = player;
        player->stand_state = 0x64;
        g_pc_guess->stand_state = 0x64;
        EQ_GLOBAL(u32, 0x006E4DA4) = 0;
        g_control_player_guess = g_local_player_guess;
        EQ_METHOD(1, void, 0x0040E422, (void*, int), g_display_guess)(HWM_FIELD(int, player->actor_info, 0));
        EQ_GLOBAL(u32, 0x006B3190) = 1;
        break;
    case 0x209A:
        if (*(int*)(data + 4) <= 0)
            break;
        if (g_pc_guess == NULL)
            break;
        sprintf(text, EQ_ARRAY(char, 0x0055FDD8));
        HWM_DSP_CHAT(text, 0xF);
        EQ_METHOD(1, void, 0x0040240F, (void*, int), g_pc_guess)(*(int*)(data + 4));
        break;
    case 0x2091:
        player = HWM_GET_PLAYER((const char*)data);
        if (player == NULL)
            break;
        savedRace = player->race;
        savedTexture = (s8)player->texture;
        savedHelm = player->equipment[0];
        savedFace = player->face;
        savedGender = player->gender;
        if (player->actor_info == NULL)
            break;
        if (HWM_FIELD(int, player->actor_info, 0) == 0)
            break;
        player->gender = data[0x40];
        if (*(short*)(data + 0x3E) != -1)
            player->race = *(short*)(data + 0x3E);
        player->texture = data[0x42];
        if (*(short*)(data + 0x44) != -1)
            player->equipment[0] = data[0x44];
        if (*(short*)(data + 0x46) != -1)
            player->face = data[0x46];
        if (*(short*)(data + 0x3E) != 0x8F && *(short*)(data + 0x3E) != 0x8E)
        {
            EQ_METHOD(1, void, 0x004B156C, (EQPlayer*, char*), player)(actorTag);
            strcat(actorTag, EQ_ARRAY(char, 0x0053E4B0));
        }
        else
        {
            if (EQ_METHOD(3, char, 0x0040C17D, (void*, char*, int, EQPlayer*), g_display_guess)(
                    nearestTag, (*(short*)(data + 0x3E) == 0x8F) + 7, player) != 1)
            {
                player->race = savedRace;
                player->texture = (u8)savedTexture;
                player->equipment[0] = (u8)savedHelm;
                player->face = (u8)savedFace;
                player->gender = savedGender;
                if (player != g_local_player_guess)
                    break;
                EQ_METHOD(1, void, 0x00424E22, (void*, void*), g_pc_guess)(
                    EQ_METHOD(2, void*, 0x0042381B, (void*, int, short*), g_pc_guess)(0x3A, &affectSlot));
                if (affectSlot != -1)
                    EQ_METHOD(1, void, 0x004251B9, (void*, int), g_pc_guess)(affectSlot);
                HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055FDB4), 0x108);
                break;
            }
            EQ_CDECL(void, 0x004A0841, (char*, const char*, int))(actorTag, nearestTag, 0x1D);
        }
        if (HWM_FIELD(int, player->actor_info, 0) != 0)
        {
            EQ_METHOD(1, void, 0x00411715, (void*, int), g_display_guess)(HWM_FIELD(int, player->actor_info, 0));
            HWM_FIELD(int, player->actor_info, 0) = 0;
        }
        strcpy((char*)player->actor_info + 8, actorTag);
        HWM_FIELD(int, player->actor_info, 0x198) = 0x20;
        HWM_FIELD(int, player->actor_info, 0x180) = 0xFE;
        HWM_FIELD(u8, player->actor_info, 0x184) = 0;
        HWM_FIELD(int, player->actor_info, 0x178) = 0;
        HWM_FIELD(int, player->actor_info, 0x250) = 0;
        HWM_FIELD(int, player->actor_info, 0x254) = 0;
        HWM_FIELD(int, player->actor_info, 0x258) = 0;
        HWM_FIELD(int, player->actor_info, 0x25C) = 0;
        HWM_FIELD(int, player->actor_info, 0x260) = 0;
        HWM_FIELD(int, player->actor_info, 0x264) = 0;
        HWM_FIELD(int, player->actor_info, 0x26C) = 0;
        player->size = -1.0f;
        EQ_METHOD(0, void, 0x004BC9BA, (EQPlayer*), player)();
        EQ_METHOD(0, void, 0x004B4873, (EQPlayer*), player)();
        HWM_FIELD(float, player->actor_info, 0x48) =
            EQ_METHOD(3, float, 0x00414245, (void*, float, float, float), g_display_guess)(player->y, player->x, player->z);
        if (HWM_FIELD(float, player->actor_info, 0x48) == EQ_GLOBAL(float, 0x005351F4))
            HWM_FIELD(float, player->actor_info, 0x48) = 1.0f;
        HWM_FIELD(int, player->actor_info, 0x68) = 0;
        EQ_METHOD(1, void, 0x0040F731, (void*, EQPlayer*), g_display_guess)(player);
        EQ_METHOD(0, void, 0x004B0D95, (EQPlayer*), player)();
        HWM_UPDATE_ACTOR(player);
        HWM_FIELD(int, player->actor_info, 0x68) = 0;
        if (player != g_local_player_guess)
            break;
        EQ_METHOD(1, void, 0x00413ED6, (void*, char*), g_display_guess)(g_local_player_guess->name);
        if (g_pc_guess == NULL)
            break;
        EQ_METHOD(1, void, 0x004226C1, (void*, int), g_pc_guess)(0);
        break;
    case 0x2093:
        if (g_pc_guess == NULL)
            break;
        if (g_local_player_guess == NULL)
            break;
        if (*(short*)(data + 4) != 0 && *(short*)(data + 4) != 3)
        {
            EQ_GLOBAL(short, 0x006B6772) = -1;
            EQ_GLOBAL(u32, 0x006B6780) = 0;
            EQ_METHOD(1, void, 0x004B45CB, (EQPlayer*, int), g_local_player_guess)(0x64);
        }
        if (*(short*)(data + 4) == 0 || *(short*)(data + 4) == 1)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055FCE0), 0xF);
        }
        else if (*(short*)(data + 4) == 3)
        {
            if (HWM_EQMAIN == NULL)
                break;
            if (HWM_FIELD(int, HWM_EQMAIN, 0xE5C) > 0)
                HWM_FIELD(int, HWM_EQMAIN, 0xE5C)--;
            else
                HWM_FIELD(int, HWM_EQMAIN, 0xE5C) = 0;
            goto combine_finished;
        }
        else if (*(short*)(data + 4) == 4)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055FD8C), 0xF);
        }
        else if (*(short*)(data + 4) == 5)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055FD64), 0xF);
        }
        else if (*(short*)(data + 4) == 6)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055FD34), 0xF);
        }
        else if (*(short*)(data + 4) == 7)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055FCFC), 0xF);
        }
        break;
    case 0x2105:
    combine_finished:
        if (HWM_FIELD(int, HWM_EQMAIN, 0x94) > 0)
            HWM_FIELD(int, HWM_EQMAIN, 0x94)--;
        else
            HWM_FIELD(int, HWM_EQMAIN, 0x94) = 0;
        break;
    case 0x20B6:
        if (HWM_EQMAIN == NULL)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        memcpy(HWM_EQMAIN + 0x101C, data, 0x414);
        player = HWM_GET_PLAYER_BY_ID(HWM_FIELD(int, HWM_EQMAIN, 0x1020));
        if (player == NULL)
            break;
        if (EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float), g_display_guess)(
                player, g_control_player_guess, 1.0f) < EQ_GLOBAL(float, 0x00534498))
        {
            if (g_game_state_guess == 2 &&
                (g_pcloudFlagA_guess == 0 || EQ_GLOBAL(int, 0x006B63FC) == 0x280))
            {
                EQ_GLOBAL(int, 0x00590170) = 2;
                g_game_state_guess = 1;
                HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
                EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
            }
            ((HWMWindow_guess*)HWM_EQMAIN)->Deactivate_guess();
            HWM_EQMAIN[0xFE1] = HWM_EQMAIN[0xFE0];
            HWM_EQMAIN[0xFE0] = 4;
            HWM_EQMAIN[0x100C] = 1;
            HWM_FIELD(EQPlayer*, HWM_EQMAIN, 0x84) = player;
            if (HWM_IS_OPEN(HWM_WINDOW(0x006E4E60)) == 0)
                HWM_FIELD(int, HWM_EQMAIN, 0x8FBA4) = 1;
            ((HWMWindow_guess*)HWM_EQMAIN)->Activate_guess();
            if (player != g_local_player_guess)
            {
                sprintf(text, EQ_ARRAY(char, 0x0055FCC8), player->name);
                HWM_DSP_CHAT(text, 0xA);
                break;
            }
            g_chatInputInactive_guess = 0;
            EQ_METHOD(1, void, 0x004CF7A1, (void*, int), g_everquest_guess)(0);
            HWM_FIELD(u8, this, 0xBA8) = 0;
            break;
        }
        sprintf(text, EQ_ARRAY(char, 0x0055FC9C), player->name);
        HWM_DSP_CHAT(text, 0x111);
        break;
    case 0x2115:
        if (HWM_EQMAIN == NULL)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        if (*(int*)data == 0)
            break;
        HWM_FIELD(short, HWM_EQMAIN, 0x102) = *(short*)(data + 4);
        found = 0;
        HWM_FIELD(short, HWM_EQMAIN, 0x104) = *(short*)data;
        for (slot = 0; slot < 30; slot++)
        {
            if (found != 0)
                break;
            item = (u8*)g_pc_guess->inventory_items[slot];
            if (item == NULL)
                continue;
            if (item[0x81] == 1)
            {
                for (index = 0; index < item[0xD5]; index++)
                {
                    if (found == 0)
                    {
                        object = HWM_FIELD(u8*, item, 0xAC + index * 4);
                        if (object != NULL && object[0x81] == 0 &&
                            HWM_FIELD(short, object, 0x82) == HWM_FIELD(short, HWM_EQMAIN, 0x102))
                        {
                            object[0xD8] = 1;
                            object[0xDA] = 0;
                            HWM_FIELD(short, object, 0xDC) = -1;
                            found = 1;
                            HWM_FIELD(short, object, 0xE0) = HWM_FIELD(short, HWM_EQMAIN, 0x104);
                        }
                    }
                }
            }
            else if (item[0x81] == 0)
            {
                if (HWM_FIELD(short, item, 0x82) == HWM_FIELD(short, HWM_EQMAIN, 0x102))
                {
                    item[0xD8] = 1;
                    item[0xDA] = 0;
                    HWM_FIELD(short, item, 0xDC) = -1;
                    found = 1;
                    HWM_FIELD(short, item, 0xE0) = HWM_FIELD(short, HWM_EQMAIN, 0x104);
                }
            }
        }
        break;
    case 0x20B5:
        *(int*)(inspectAnswer + 0) = *(int*)(data + 4);
        *(int*)(inspectAnswer + 4) = g_local_player_guess->spawn_id;
        strcpy((char*)inspectAnswer + 0x2F4, EQ_ARRAY(char, 0x0058FBA4));
        strcpy((char*)inspectAnswer + 0x2F4, (const char*)HWM_EQMAIN + 0xEDA);
        strcpy((char*)inspectAnswer + 0x3F4, g_pc_guess->unknown0x0A78);
        for (slot = 0; slot < 22; slot++)
        {
            strcpy((char*)inspectAnswer + 8 + slot * 0x20, EQ_ARRAY(char, 0x0058FBA4));
            *(short*)(inspectAnswer + 0x2C8 + slot * 2) = -1;
            if (g_pc_guess != NULL && g_pc_guess->inventory_items[slot] != NULL)
            {
                strcpy((char*)inspectAnswer + 8 + slot * 0x20, g_pc_guess->inventory_items[slot]->name);
                *(short*)(inspectAnswer + 0x2C8 + slot * 2) = g_pc_guess->inventory_items[slot]->icon_nr;
            }
        }
        player = HWM_GET_PLAYER_BY_ID(*(int*)(data + 4));
        if (player != NULL && player != g_local_player_guess && EQ_GLOBAL(int, 0x006E2070) == 1)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055FC78), player->name);
            HWM_DSP_CHAT(text, 0x111);
        }
        HWM_SEND(0x20B6, inspectAnswer, 0x414, 1);
        break;
    case 0x20CE:
        if (HWM_WINDOW(0x006E4E60) == NULL)
            break;
        if (HWM_FIELD(int, HWM_WINDOW(0x006E4E60), 0x37AC) == 0)
            break;
        if (HWM_IS_OPEN(HWM_WINDOW(0x006E4E60)) != 0)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        strcpy((char*)HWM_WINDOW(0x006E4E60) + 0x9C, (const char*)data);
        HWM_WINDOW(0x006E4E60)->Activate_guess();
        break;
    case 0x20D5:
        EQ_CDECL(void, 0x004966B6, (u8*))(data);
        break;
    case 0x210A:
        if (HWM_EQMAIN == NULL)
            break;
        object = (u8*)new BBoardAllocation_guess(0);
        HWM_FIELD(int, object, 8) = *(int*)data;
        strcpy((char*)object + 0xC, (const char*)data + 4);
        strcpy((char*)object + 0x1C, (const char*)data + 0x14);
        strcpy((char*)object + 0x3B, (const char*)data + 0x36);
        object[0x3A] = data[0x34];
        HWM_FIELD(short, HWM_EQMAIN, 0xF4)++;
        break;
    case 0x210E:
        if (HWM_EQMAIN == NULL)
            break;
        if (g_game_state_guess == 2)
        {
            EQ_GLOBAL(int, 0x00590170) = 2;
            g_game_state_guess = 1;
            HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        }
        ((HWMWindow_guess*)HWM_EQMAIN)->Deactivate_guess();
        HWM_EQMAIN[0xFE0] = 9;
        EQ_CDECL(void, 0x004E1D09, (short))(HWM_FIELD(short, HWM_EQMAIN, 0xDAC));
        HWM_FIELD(short, HWM_EQMAIN, 0xF2) = 0;
        HWM_FIELD(short, HWM_EQMAIN, 0xFC) = 0;
        object = EQ_GLOBAL(u8*, 0x006E4D70);
        for (slot = 0; slot < 20; slot++)
        {
            HWM_FIELD(u8*, HWM_EQMAIN, 0x138 + slot * 4) = NULL;
            if (object != NULL)
            {
                HWM_FIELD(u8*, HWM_EQMAIN, 0x138 + slot * 4) = object;
                object = HWM_FIELD(u8*, object, 4);
            }
        }
    board_show:
        HWM_EQMAIN[0xDAE] = 0;
        object = HWM_FIELD(u8*, HWM_EQMAIN, 0x138 + HWM_FIELD(short, HWM_EQMAIN, 0xFC) * 4);
        if (object != NULL && strcmp((const char*)object + 0x1C, g_pc_guess->name) == 0)
            HWM_EQMAIN[0xDAE] = 1;
        HWM_EQMAIN[0x100] = 0;
        ((HWMWindow_guess*)HWM_EQMAIN)->Activate_guess();
        break;
    case 0x210B:
        if (HWM_EQMAIN == NULL)
            break;
        object = HWM_FIELD(u8*, HWM_EQMAIN, 0x138 + HWM_FIELD(short, HWM_EQMAIN, 0xFC) * 4);
        if (object != NULL && object[0x3A] < 0x20)
            EQ_CDECL(void, 0x004AFF13, (u8*, char*, u8))(
                data, (char*)HWM_EQMAIN + 0x1A6, g_pc_guess->languages[object[0x3A]]);
        else
            strcpy((char*)HWM_EQMAIN + 0x1A6, (const char*)data);
        ((HWMWindow_guess*)HWM_EQMAIN)->Deactivate_guess();
        HWM_EQMAIN[0xFE0] = 0xA;
        goto board_show;
    case 0x20D2:
        if (g_local_player_guess == NULL)
            break;
        if (g_pc_guess == NULL)
            break;
        if (*(u32*)data != g_local_player_guess->spawn_id)
            break;
        EQ_CDECL(void, 0x004A1147, (CharacterProfile*, int))(g_pc_guess, data[4]);
        g_local_player_guess->level = g_pc_guess->level;
        EQ_METHOD(0, void, 0x004BC963, (EQPlayer*), g_local_player_guess)();
        break;
    case 0x2111:
        if (HWM_EQMAIN == NULL)
            break;
        if (HWM_EQMAIN[0x101] == 0)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        for (slot = 0; slot < 4; slot++)
            HWM_FIELD(int, HWM_EQMAIN, 0x128 + slot * 4) = *(int*)(data + slot * 4);
        break;
    case 0x2114:
        sprintf(text, EQ_ARRAY(char, 0x0055FC50), data + 0x40, data + 0x20);
        HWM_DSP_CHAT(text, 0xF);
        break;
    case 0x2112:
        if (HWM_EQMAIN == NULL)
            break;
        if (HWM_EQMAIN[0x101] == 0)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        switch ((s8)data[4])
        {
        case 0:
            newItem = (EQ_Item*)new EquipmentAllocation_guess;
            memcpy(newItem, data + 5, 0x124);
            if (newItem->item_nr == 0 || ((u8*)newItem)[0xC2] == 0xD)
            {
                HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
            merchant_delete_item:
                operator delete(newItem);
                break;
            }
        merchant_place_item:
            if (EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
            {
                sprintf(text, EQ_ARRAY(char, 0x0055E2DC), newItem->item_nr);
                HWM_DSP_CHAT(text, 0xD);
                newItem->icon_nr = 500;
            }
            if (newItem->equip_slot < 30)
            {
                HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x108 + newItem->equip_slot * 4) = newItem;
                break;
            }
            index = (short)(newItem->equip_slot - 30);
            object = HWM_FIELD(u8*, HWM_EQMAIN, 0x108 + (short)(index / 10) * 4);
            if (object != NULL)
                HWM_FIELD(EQ_Item*, object, 0xAC + (short)(index % 10) * 4) = newItem;
            break;
        case 1:
            newItem = (EQ_Item*)new ContainerAllocation_guess;
            memcpy(newItem, data + 5, 0xDC);
            if (newItem->item_nr == 0)
            {
                HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
                if (newItem != NULL)
                {
                    EQ_METHOD(0, void, 0x0043405A, (void*), newItem)();
                    operator delete(newItem);
                }
                break;
            }
            ((u8*)newItem)[0xD6] = 0;
            for (index = 0; index < 10; index++)
                HWM_FIELD(EQ_Item*, newItem, 0xAC + index * 4) = NULL;
            if (EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
            {
                sprintf(text, EQ_ARRAY(char, 0x0055E2F8), newItem->item_nr);
                HWM_DSP_CHAT(text, 0xD);
                newItem->icon_nr = 500;
            }
            HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x108 + newItem->equip_slot * 4) = newItem;
            break;
        case 2:
            newItem = (EQ_Item*)new NoteAllocation_guess;
            memcpy(newItem, data + 5, 0xD0);
            if (newItem->item_nr != 0)
                goto merchant_place_item;
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
            if (newItem == NULL)
                break;
            EQ_METHOD(0, void, 0x0040101D, (void*), newItem)();
            goto merchant_delete_item;
        }
        EQ_CDECL(void, 0x004A2816, (CharacterProfile*))(g_pc_guess);
        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        break;
    case 0x20D7:
        if (HWM_FIELD(int, HWM_EQMAIN, 0x94) > 0)
            HWM_FIELD(int, HWM_EQMAIN, 0x94)--;
        else
            HWM_FIELD(int, HWM_EQMAIN, 0x94) = 0;
        if (g_local_player_guess != NULL && g_local_player_guess->spawn_id == *(u32*)data && HWM_EQMAIN != NULL)
        {
            if (*(short*)(data + 8) == 0)
            {
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), g_everquest_guess)(EQ_ARRAY(char, 0x0055FBE4));
            }
            else if (HWM_FIELD(u8*, HWM_EQMAIN, 0x1010) != NULL)
            {
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), g_everquest_guess)(EQ_ARRAY(char, 0x0055FC14));
            }
            else
            {
                HWM_FIELD(u8*, HWM_EQMAIN, 0x1010) = (u8*)new ContainerAllocation_guess;
                HWM_FIELD(u8*, HWM_EQMAIN, 0x1010)[0xD5] = data[0xC];
                HWM_FIELD(short, HWM_FIELD(u8*, HWM_EQMAIN, 0x1010), 0x84) = *(short*)(data + 0x10);
                HWM_FIELD(u8*, HWM_EQMAIN, 0x1010)[0xD4] = data[0xA];
                HWM_FIELD(u8*, HWM_EQMAIN, 0x1010)[0xD6] = 1;
                HWM_FIELD(u8*, HWM_EQMAIN, 0x1010)[0xD7] = 0xA;
                HWM_FIELD(u8*, HWM_EQMAIN, 0x1010)[0xD8] = 0;
                HWM_FIELD(int, HWM_EQMAIN, 0x1014) = *(int*)(data + 4);
                for (slot = 0; slot < 10; slot++)
                    HWM_FIELD(int, HWM_FIELD(u8*, HWM_EQMAIN, 0x1010), 0xAC + slot * 4) = 0;
                if (HWM_IS_OPEN(HWM_WINDOW(0x006E4E58)) != 0)
                {
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E58), 0x174)[0xD6] = 0;
                    HWM_WINDOW(0x006E4E58)->Deactivate_guess();
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E58), 0x174) = NULL;
                }
                HWM_FIELD(u8*, HWM_WINDOW(0x006E4E58), 0x174) = HWM_FIELD(u8*, HWM_EQMAIN, 0x1010);
                HWM_WINDOW(0x006E4E58)->Show_guess(-1);
                break;
            }
        }
        HWM_FIELD(int, HWM_EQMAIN, 0xE5C) = 0;
        break;
    case 0x20FB:
        if (HWM_EQMAIN == NULL)
            break;
        if (HWM_FIELD(u8*, HWM_EQMAIN, 0x1010) == NULL)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        newItem = (EQ_Item*)new EquipmentAllocation_guess;
        memcpy(newItem, data, 0x124);
        if (newItem->item_nr == 0 || ((u8*)newItem)[0xC2] == 0xD)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
            operator delete(newItem);
            break;
        }
        if (EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E2DC), newItem->item_nr);
            HWM_DSP_CHAT(text, 0xD);
            newItem->icon_nr = 500;
        }
    station_place_item:
        if (newItem->equip_slot >= 0 && newItem->equip_slot < 10)
        {
            object = HWM_FIELD(u8*, HWM_EQMAIN, 0x1010);
            if (object != NULL && newItem->equip_slot < object[0xD5])
                HWM_FIELD(EQ_Item*, object, 0xAC + newItem->equip_slot * 4) = newItem;
        }
        if (g_pc_guess->inventory_items[0] != NULL)
            g_displayManager_guess[0x3C] = 1;
        else
            g_displayManager_guess[0x3C] = 0;
        break;
    case 0x20FC:
        if (HWM_EQMAIN == NULL)
            break;
        if (HWM_FIELD(u8*, HWM_EQMAIN, 0x1010) == NULL)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        newItem = (EQ_Item*)new NoteAllocation_guess;
        memcpy(newItem, data, 0xD0);
        if (newItem->item_nr == 0)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
            if (newItem == NULL)
                break;
            EQ_METHOD(0, void, 0x0040101D, (void*), newItem)();
            operator delete(newItem);
            break;
        }
        if (EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E6E8), newItem->item_nr);
            HWM_DSP_CHAT(text, 0xD);
            newItem->icon_nr = 500;
        }
        goto station_place_item;
    case 0x207B:
        if (g_pc_guess == NULL)
            break;
        strcpy((char*)itemMessage, (const char*)data + 0x1E);
        strcpy((char*)itemMessage + 0x1E, g_local_player_guess->name);
        for (slot = 0; slot < 30; slot++)
        {
            *(short*)(itemMessage + 0x3C) = (short)slot;
            item = (u8*)g_pc_guess->inventory_items[slot];
            if (item == NULL)
            {
                itemMessage[0x3E] = 0xFF;
                HWM_SEND(0x207C, itemMessage, 0x40, 1);
                continue;
            }
            itemMessage[0x3E] = item[0x81];
            if (item[0x81] == 0)
            {
                messageSize = 0x164;
                memcpy(itemMessage + 0x3F, g_pc_guess->inventory_items[slot], 0x124);
            }
            else if (item[0x81] == 1)
            {
                messageSize = 0x11C;
                memcpy(itemMessage + 0x3F, g_pc_guess->inventory_items[slot], 0xDC);
            }
            else if (item[0x81] == 2)
            {
                messageSize = 0x110;
                memcpy(itemMessage + 0x3F, g_pc_guess->inventory_items[slot], 0xD0);
            }
            HWM_SEND(0x207C, itemMessage, messageSize, 1);
            if (itemMessage[0x3E] != 1)
                continue;
            item = (u8*)g_pc_guess->inventory_items[slot];
            for (index = 0; index < item[0xD5]; index++)
            {
                object = HWM_FIELD(u8*, item, 0xAC + index * 4);
                if (object == NULL)
                    continue;
                itemMessage[0x3E] = object[0x81];
                *(short*)(itemMessage + 0x3C) = (short)(index + (slot * 5 + 0xF) * 2);
                if (object[0x81] == 0)
                {
                    messageSize = 0x164;
                    memcpy(itemMessage + 0x3F, HWM_FIELD(u8*, item, 0xAC + index * 4), 0x124);
                }
                else if (object[0x81] == 2)
                {
                    messageSize = 0x110;
                    memcpy(itemMessage + 0x3F, HWM_FIELD(u8*, item, 0xAC + index * 4), 0xD0);
                }
                HWM_SEND(0x207C, itemMessage, messageSize, 1);
            }
        }
        for (slot = 0; slot < 8; slot++)
        {
            *(short*)(itemMessage + 0x3C) = (short)(slot + 2000);
            item = (u8*)g_pc_guess->bank_items[slot];
            if (item == NULL)
            {
                itemMessage[0x3E] = 0xFF;
                HWM_SEND(0x207C, itemMessage, 0x40, 1);
                continue;
            }
            itemMessage[0x3E] = item[0x81];
            if (item[0x81] == 0)
            {
                messageSize = 0x164;
                memcpy(itemMessage + 0x3F, g_pc_guess->bank_items[slot], 0x124);
            }
            else if (item[0x81] == 1)
            {
                messageSize = 0x11C;
                memcpy(itemMessage + 0x3F, g_pc_guess->bank_items[slot], 0xDC);
            }
            else if (item[0x81] == 2)
            {
                messageSize = 0x110;
                memcpy(itemMessage + 0x3F, g_pc_guess->bank_items[slot], 0xD0);
            }
            HWM_SEND(0x207C, itemMessage, messageSize, 1);
            if (itemMessage[0x3E] != 1)
                continue;
            item = (u8*)g_pc_guess->bank_items[slot];
            for (index = 0; index < item[0xD5]; index++)
            {
                object = HWM_FIELD(u8*, item, 0xAC + index * 4);
                if (object == NULL)
                    continue;
                itemMessage[0x3E] = object[0x81];
                *(short*)(itemMessage + 0x3C) = (short)(index + (slot * 5 + 0x3F7) * 2);
                if (object[0x81] == 0)
                {
                    messageSize = 0x164;
                    memcpy(itemMessage + 0x3F, HWM_FIELD(u8*, item, 0xAC + index * 4), 0x124);
                }
                else if (object[0x81] == 2)
                {
                    messageSize = 0x110;
                    memcpy(itemMessage + 0x3F, HWM_FIELD(u8*, item, 0xAC + index * 4), 0xD0);
                }
                HWM_SEND(0x207C, itemMessage, messageSize, 1);
            }
        }
        strcpy((char*)coinMessage, (const char*)data + 0x1E);
        strcpy((char*)coinMessage + 0x1E, g_local_player_guess->name);
        coinMessage[0x3C] = 1;
        *(int*)(coinMessage + 0x40) = g_pc_guess->platinum;
        HWM_SEND(0x2104, coinMessage, 0x44, 1);
        coinMessage[0x3C] = 2;
        *(int*)(coinMessage + 0x40) = g_pc_guess->gold;
        HWM_SEND(0x2104, coinMessage, 0x44, 1);
        coinMessage[0x3C] = 3;
        *(int*)(coinMessage + 0x40) = g_pc_guess->silver;
        HWM_SEND(0x2104, coinMessage, 0x44, 1);
        coinMessage[0x3C] = 4;
        *(int*)(coinMessage + 0x40) = g_pc_guess->copper;
        HWM_SEND(0x2104, coinMessage, 0x44, 1);
        coinMessage[0x3C] = 5;
        *(int*)(coinMessage + 0x40) = g_pc_guess->platinum_bank;
        HWM_SEND(0x2104, coinMessage, 0x44, 1);
        coinMessage[0x3C] = 6;
        *(int*)(coinMessage + 0x40) = g_pc_guess->gold_bank;
        HWM_SEND(0x2104, coinMessage, 0x44, 1);
        coinMessage[0x3C] = 7;
        *(int*)(coinMessage + 0x40) = g_pc_guess->silver_bank;
        HWM_SEND(0x2104, coinMessage, 0x44, 1);
        coinMessage[0x3C] = 8;
        *(int*)(coinMessage + 0x40) = g_pc_guess->copper_bank;
        HWM_SEND(0x2104, coinMessage, 0x44, 1);
        break;
    case 0x2122:
        object = EQ_METHOD(1, u8*, 0x004BDCC7, (void*, char), EQ_GLOBAL(void*, 0x006E4D78))((char)data[0]);
        if (object == NULL)
            break;
        HWM_FIELD(short, object, 0x62) = *(short*)(data + 4);
        break;
    case 0x2126:
        player = HWM_GET_PLAYER_BY_ID(*(int*)data);
        if (player == NULL)
            break;
        if (data[4] != 1)
            break;
        if (g_control_player_guess != NULL)
        {
            g_control_player_guess->speed = 0.0f;
            g_control_player_guess->delta_heading = 0.0f;
        }
        if (player->race == 0x8D)
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055FBC8), 0x111);
        g_control_player_guess = player;
        break;
    case 0x20A0:
        EQ_CDECL(void, 0x004960F2, (u8*))(data);
        break;
    case 0x2107:
        if (_strcmpi((const char*)data + 0x20, g_pc_guess->name) == 0)
        {
            if (*(short*)(data + 0x42) != 7 && *(short*)(data + 0x42) != 0xE)
                break;
            if (isdigit((s8)data[strlen((const char*)data) - 1]) != 0)
                sprintf(chatText, EQ_ARRAY(char, 0x0055FBB4), HWM_STRIP_NAME((const char*)data),
                    HWM_STRIP_NAME((const char*)data + 0x46));
            else
                sprintf(chatText, EQ_ARRAY(char, 0x0055FBB4), data, data + 0x46);
            HWM_DSP_CHAT(chatText, 0x111);
            break;
        }
        if (EQ_ARRAY(char, 0x006B64A8)[0] != 0)
        {
            strcpy((char*)relayMessage, EQ_ARRAY(char, 0x006B64A8));
            strcpy((char*)relayMessage + 0x40, (const char*)data + 0x20);
            strcpy((char*)relayMessage + 0x20, (const char*)data);
            strcpy((char*)relayMessage + 0x62, (const char*)data + 0x46);
            *(short*)(relayMessage + 0x60) = *(short*)(data + 0x42);
            messageSize = strlen((const char*)relayMessage + 0x62) + 0x67;
            if (_strcmpi((const char*)data + 0x20, EQ_ARRAY(char, 0x006B64A8)) != 0 ||
                _strcmpi((const char*)data, EQ_ARRAY(char, 0x006B64A8)) != 0)
                HWM_SEND(0x2133, relayMessage, messageSize, 1);
        }
        if (*(short*)(data + 0x42) < 6 && g_filter_6984_guess[*(short*)(data + 0x42)] == 0)
            break;
        player = g_local_player_guess;
        if (*(short*)(data + 0x42) == 0xA || *(short*)(data + 0x42) == 0xB)
        {
            if (player != NULL && player->gm == 0)
                break;
            if (player->gm != 0 && EQ_GLOBAL(char, 0x006B752A) != 0)
                break;
        }
        if (*(short*)(data + 0x42) != 6 && *(short*)(data + 0x42) != 0xE)
        {
            for (index = 0; index < 30; index++)
            {
                if (_strcmpi((const char*)data + 0x20, &g_ignore_list_guess[index * 0x20]) == 0)
                {
                    player = HWM_GET_PLAYER((const char*)data + 0x20);
                    if (player == NULL)
                        goto world_done;
                    if (player->gm == 0)
                        goto world_done;
                    g_ignore_list_guess[index * 0x20] = 0;
                    break;
                }
            }
            player = g_local_player_guess;
        }
        color = 0x111;
        if (*(short*)(data + 0x42) != 6 && player->gm == 0)
        {
            EQ_METHOD(2, void, 0x004501A1, (void*, u8, u8), g_pc_guess)(data[0x40], data[0x44]);
            EQ_CDECL(void, 0x004AF87F, (u8*, char*, int, int, int))(
                data + 0x46, babelText, data[0x40], data[0x44], EQ_GLOBAL(int, 0x006B68B4));
            EQ_CDECL(void, 0x004AFF13, (char*, u8*, u8))(
                babelText, data + 0x46, g_pc_guess->languages[*(short*)(data + 0x40)]);
        }
        if (*(short*)(data + 0x40) != 0)
        {
            if (g_pc_guess->languages[*(short*)(data + 0x40)] > 0x18)
                sprintf(languageText, EQ_ARRAY(char, 0x0055FBAC),
                    EQ_METHOD(1, char*, 0x004C8FCE, (void*, short), g_everquest_guess)(*(short*)(data + 0x40)));
            else
                strcpy(languageText, EQ_ARRAY(char, 0x0055FB94));
        }
        else
        {
            languageText[0] = 0;
        }
        switch (*(short*)(data + 0x42))
        {
        case 0:
            color = 0x103;
            sprintf(chatText, EQ_ARRAY(char, 0x0055FB14), data + 0x20, languageText, data + 0x46);
            break;
        case 2:
            color = 0x102;
            sprintf(chatText, EQ_ARRAY(char, 0x0055FB30), data + 0x20, languageText, data + 0x46);
            break;
        case 3:
            color = 0x106;
            sprintf(chatText, EQ_ARRAY(char, 0x0055FB4C), data + 0x20, languageText, data + 0x46);
            break;
        case 4:
            color = 0x105;
            sprintf(chatText, EQ_ARRAY(char, 0x0055FB60), data + 0x20, languageText, data + 0x46);
            break;
        case 5:
            color = 0x104;
            sprintf(chatText, EQ_ARRAY(char, 0x0055FB74), data + 0x20, data + 0x46);
            break;
        case 6:
            color = 0x10D;
            sprintf(chatText, EQ_ARRAY(char, 0x0055FB00), data + 0x20, data + 0x46);
            break;
        case 7:
        case 14:
            sprintf(chatText, EQ_ARRAY(char, 0x0055FAB4), data + 0x20, languageText, data + 0x46);
            EQ_CDECL(void, 0x004A8181, (u8*))(data + 0x20);
            color = 0x101;
            if (g_local_player_guess->afk == 0)
                break;
            if (EQ_GLOBAL(void*, 0x006E4DE8) == NULL)
                break;
            if (strncmp((const char*)data + 0x46, EQ_ARRAY(char, 0x0055FAA0), 0x12) == 0)
                break;
            if (strncmp((const char*)data + 0x46, EQ_ARRAY(char, 0x0055FA90), 0xC) == 0)
                break;
            strcpy((char*)afkReply + 0x20, g_local_player_guess->name);
            strcpy((char*)afkReply, (const char*)data + 0x20);
            if (EQ_ARRAY(char, 0x006E1F50)[0] != 0)
            {
                strcpy((char*)afkReply + 0x46, EQ_ARRAY(char, 0x0055FA80));
                strcat((char*)afkReply + 0x46, EQ_ARRAY(char, 0x006E1F50));
            }
            else
            {
                strcpy((char*)afkReply + 0x46, EQ_ARRAY(char, 0x0055FA4C));
            }
            *(short*)(afkReply + 0x42) = 7;
            *(short*)(afkReply + 0x40) =
                HWM_FIELD(short, g_chatWindow_guess, 0x17C + EQ_GLOBAL(short, 0x006B642A) * 2);
            *(short*)(afkReply + 0x44) = g_pc_guess->languages[*(short*)(afkReply + 0x40)];
            if (g_local_player_guess->gm != 0)
                *(short*)(afkReply + 0x44) = 100;
            HWM_SEND(0x2107, afkReply, (short)(strlen((const char*)afkReply + 0x46) + 0x4B), 1);
            break;
        case 8:
            color = 0x100;
            sprintf(chatText, EQ_ARRAY(char, 0x0055FAF0),
                EQ_METHOD(1, char*, 0x004D02FD, (void*, const char*), this)((const char*)data + 0x20), languageText,
                data + 0x46);
            break;
        case 10:
            color = 0xF;
            sprintf(chatText, EQ_ARRAY(char, 0x0055FACC), data + 0x20, data + 0x46);
            break;
        case 11:
            color = 0xF;
            sprintf(chatText, EQ_ARRAY(char, 0x0055FAE0), data + 0x20, data + 0x46);
            break;
        default:
            sprintf(chatText, EQ_ARRAY(char, 0x0055FAF0), data + 0x20, languageText, data + 0x46);
            break;
        }
        HWM_DSP_CHAT(chatText, color);
        break;
    case 0x2031:
        EQ_CDECL(void, 0x00496213, (u8*))(data);
        break;
    case 0x2034:
        newItem = (EQ_Item*)new NoteAllocation_guess;
        memcpy(newItem, data, 0xD0);
        if (newItem->item_nr == 0)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
            if (newItem != NULL)
            {
                EQ_METHOD(0, void, 0x0040101D, (void*), newItem)();
                operator delete(newItem);
            }
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            errorMessage.reason = 0x19;
            goto send_client_error;
        }
        if (HWM_FIELD(int, this, 4) == 5 && EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E6E8), newItem->item_nr);
            HWM_DSP_CHAT(text, 0xD);
            newItem->icon_nr = 500;
        }
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        if (newItem->equip_slot < 30)
        {
            g_pc_guess->inventory_items[newItem->equip_slot] = newItem;
            if (newItem->equip_slot <= 0)
                goto summoned_note_done;
        summoned_note_sound:
            if (g_sound_manager_ptr_guess != NULL)
                HWM_WAVE_PLAY(0x92);
        }
        else if (newItem->equip_slot <= 0x154)
        {
            index = newItem->equip_slot - 30;
            slot = index / 10;
            if ((short)slot == 30)
                slot = 0;
            item = (u8*)g_pc_guess->inventory_items[(short)slot];
            if (item != NULL && item[0x81] == 1)
            {
                if (item != NULL)
                {
                    HWM_FIELD(EQ_Item*, item, 0xAC + (short)(index % 10) * 4) = newItem;
                    goto summoned_note_sound;
                }
                if (newItem != NULL)
                {
                    EQ_METHOD(0, void, 0x0040101D, (void*), newItem)();
                    operator delete(newItem);
                }
                errorMessage.unknown0x00 = -2;
                errorMessage.unknown0x04 = -2;
                errorMessage.reason = 0x1B;
                goto send_client_error;
            }
            if (newItem != NULL)
            {
                EQ_METHOD(0, void, 0x0040101D, (void*), newItem)();
                operator delete(newItem);
            }
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            errorMessage.reason = 0x1A;
            goto send_client_error;
        }
        else if (newItem->equip_slot >= 2000 && newItem->equip_slot < 2009)
        {
            g_pc_guess->bank_items[(short)(newItem->equip_slot - 2000)] = newItem;
        }
        else if (newItem->equip_slot >= 2030)
        {
            index = (short)(newItem->equip_slot - 2000) - 30;
            item = (u8*)g_pc_guess->bank_items[(short)(index / 10)];
            if (item != NULL && item[0x81] == 1)
            {
                if (item != NULL)
                {
                    HWM_FIELD(EQ_Item*, item, 0xAC + (short)(index % 10) * 4) = newItem;
                    goto summoned_note_done;
                }
                if (newItem != NULL)
                {
                    EQ_METHOD(0, void, 0x0040101D, (void*), newItem)();
                    operator delete(newItem);
                }
                errorMessage.reason = 0x1D;
                errorMessage.unknown0x00 = -2;
                errorMessage.unknown0x04 = -2;
                goto send_client_error;
            }
            if (newItem != NULL)
            {
                EQ_METHOD(0, void, 0x0040101D, (void*), newItem)();
                operator delete(newItem);
            }
            errorMessage.reason = 0x1C;
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            goto send_client_error;
        }
        else
        {
            if (newItem != NULL)
            {
                EQ_METHOD(0, void, 0x0040101D, (void*), newItem)();
                operator delete(newItem);
            }
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            errorMessage.reason = 0x1E;
            goto send_client_error;
        }
    summoned_note_done:
        EQ_CDECL(void, 0x004A2816, (CharacterProfile*))(g_pc_guess);
        if (g_pc_guess->inventory_items[0] == NULL)
            break;
        g_displayManager_guess[0x3C] = 1;
        break;
    case 0x2030:
        newItem = (EQ_Item*)new ContainerAllocation_guess;
        memcpy(newItem, data, 0xDC);
        if (newItem->item_nr == 0)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
            if (newItem != NULL)
            {
                EQ_METHOD(0, void, 0x0043405A, (void*), newItem)();
                operator delete(newItem);
            }
            errorMessage.reason = 0x1F;
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            goto send_client_error;
        }
        if (HWM_FIELD(int, this, 4) == 5 && EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E2F8), newItem->item_nr);
            HWM_DSP_CHAT(text, 0xD);
            newItem->icon_nr = 500;
        }
        ((u8*)newItem)[0xD6] = 0;
        for (index = 0; index < 10; index++)
            HWM_FIELD(EQ_Item*, newItem, 0xAC + index * 4) = NULL;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        if (newItem->equip_slot < 30)
        {
            if (g_pc_guess->inventory_items[newItem->equip_slot] == NULL)
            {
                g_pc_guess->inventory_items[newItem->equip_slot] = newItem;
                if (newItem->equip_slot > 0 && g_sound_manager_ptr_guess != NULL)
                    HWM_WAVE_PLAY(0x92);
                goto trade_item_placed;
            }
            if (newItem != NULL)
            {
                EQ_METHOD(0, void, 0x0043405A, (void*), newItem)();
                operator delete(newItem);
            }
            errorMessage.reason = 0x20;
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            goto send_client_error;
        }
        if (newItem->equip_slot >= 2000 && newItem->equip_slot < 2009)
        {
            if (g_pc_guess->bank_items[(short)(newItem->equip_slot - 2000)] == NULL)
            {
                g_pc_guess->bank_items[(short)(newItem->equip_slot - 2000)] = newItem;
                goto trade_item_placed;
            }
            if (newItem != NULL)
            {
                EQ_METHOD(0, void, 0x0043405A, (void*), newItem)();
                operator delete(newItem);
            }
            errorMessage.reason = 0x21;
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            goto send_client_error;
        }
        if (newItem != NULL)
        {
            EQ_METHOD(0, void, 0x0043405A, (void*), newItem)();
            operator delete(newItem);
        }
        errorMessage.reason = 0x22;
        errorMessage.unknown0x00 = -2;
        errorMessage.unknown0x04 = -2;
        goto send_client_error;
    case 0x201D:
        DbgLog_guess(EQ_ARRAY(char, 0x0055FA18));
        break;
    case 0x202D:
        if (size <= 4)
        {
            DbgLog_guess(EQ_ARRAY(char, 0x0055F9F4));
            goto corrupt_profile;
        }
        EQ_CDECL(void, 0x004A3977, (u8*, u32))(data, size);
        g_profile_stream_guess->avail_in = size;
        g_profile_stream_guess->next_in = data;
        g_profile_stream_guess->total_in = 0;
        g_profile_stream_guess->next_out = (u8*)g_profile_buffer_guess;
        g_profile_stream_guess->avail_out = 0x4000;
        g_profile_stream_guess->total_out = 0;
        g_profile_stream_guess->data_type = 0;
        inflateInit2__guess(
            g_profile_stream_guess, 0xF, EQ_ARRAY(char, 0x0055DD70), sizeof(ZStream_guess));
        inflate_guess(g_profile_stream_guess, 4);
        size = g_profile_stream_guess->total_out;
        inflateEnd_guess(g_profile_stream_guess);
        if (size != sizeof(MsgPlayerProfile) ||
            EQ_CDECL(u32, 0x004E04A2, (void*, int))(&g_profile_buffer_guess->profile, sizeof(CharacterProfile)) !=
                g_profile_buffer_guess->crc)
        {
            DbgLog_guess(EQ_ARRAY(char, 0x0055F9F4));
        corrupt_profile:
            HWM_FIELD(int, this, 0x223F4) = 1;
            HWM_FIELD(int, this, 4) = 0xFF;
            EQ_GLOBAL(u32, 0x006B63F4) = 0xCB;
            HWM_FIELD(int, this, 0) = 1;
            if (EQ_GLOBAL(void*, 0x006B3194) == NULL)
                break;
            HWM_CLOSE_WORLD();
            EQ_GLOBAL(void*, 0x006B3194) = NULL;
            break;
        }
        *g_pc_guess = g_profile_buffer_guess->profile;
        EQ_GLOBAL(int, 0x006E4D4C) = _strcmpi(g_pc_guess->zone, EQ_ARRAY(char, 0x0055F9E8)) == 0;
        for (slot = 0; slot < 30; slot++)
            g_pc_guess->inventory_items[slot] = NULL;
        for (slot = 0; slot < 8; slot++)
            g_pc_guess->bank_items[slot] = NULL;
        for (slot = 0; slot < 5; slot++)
        {
            g_group_names_guess[slot * 0x1E] = 0;
            g_group_slot_used_guess[slot] = 0;
            g_death_state_guess[slot] = 0;
        }
        g_pc_guess->unknown0x0076 = 0;
        break;
    case 0x20A9:
        memcpy(&beginCast, data, sizeof(MsgBeginCast));
        EQ_METHOD(1, void, 0x004C86F8, (void*, MsgBeginCast*), this)(&beginCast);
        break;
    case 0x20A2:
        memcpy(&zoneChange, data, 0x40);
        if (_strcmpi(HWM_STRIP_SHIP(g_pc_guess->boat), HWM_STRIP_SHIP(zoneChange.char_name)) != 0)
            break;
        if (EQ_METHOD(1, short, 0x004CE8B9, (void*, char*), this)(HWM_STRIP_SHIP(zoneChange.char_name)) < 0)
        {
            EQ_CDECL(int, 0x005229E8, (const char*))(EQ_ARRAY(char, 0x0055F9B8));
            break;
        }
        g_vehicle_found_guess = 1;
        EQ_METHOD(3, void, 0x004DC1B5, (void*, const char*, const char*, int), this)(
            zoneChange.zone_name, EQ_ARRAY(char, 0x0055F9DC), 0);
        EQ_METHOD(0, void, 0x0040C305, (void*), g_display_guess)();
        EQ_METHOD(0, void, 0x0040C305, (void*), g_display_guess)();
        break;
    case 0x202B:
        player = HWM_GET_PLAYER_BY_ID(*(int*)data);
        if (player == NULL)
            break;
        if (player->spawn_id == EQ_GLOBAL(u32, 0x006B64C8))
        {
            if (EQ_GLOBAL(u8, 0x006B64D0) != 0)
            {
                sprintf(text, EQ_ARRAY(char, 0x0055F96C), g_local_player_guess->name, player->name, player->name);
                HWM_DSP_CHAT(text, 0x10D);
                duelMessage[0] = player->spawn_id;
                duelMessage[1] = g_local_player_guess->spawn_id;
                HWM_SEND(0x215E, duelMessage, 8, 1);
            }
            else
            {
                sprintf(text, EQ_ARRAY(char, 0x0055F93C), player->name);
                HWM_DSP_CHAT(text, 0xF);
            }
            EQ_GLOBAL(u8, 0x006B64D0) = 0;
            EQ_GLOBAL(u32, 0x006B64C8) = 0;
            EQ_GLOBAL(u32, 0x006B64CC) = 0;
        }
        if (HWM_EQMAIN != NULL && HWM_FIELD(int, HWM_EQMAIN, 0x685E4) != 0 &&
            EQ_GLOBAL(EQPlayer*, 0x006E4D88) != NULL && EQ_GLOBAL(EQPlayer*, 0x006E4D88) == player)
            EQ_METHOD(0, void, 0x004571EB, (void*), HWM_EQMAIN)();
        if (g_name_buffer_guess[0] != 0)
        {
            for (slot = 0; slot < 5; slot++)
            {
                if (g_group_names_guess[slot * 0x1E] != 0 && g_death_state_guess[slot] != 0 &&
                    g_death_state_guess[slot] == (u32)player)
                {
                    g_death_state_guess[slot] = 0;
                    if (HWM_EQMAIN != NULL)
                        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
                }
            }
        }
        if (player == EQ_GLOBAL(EQPlayer*, 0x006E4DB8) && HWM_EQMAIN != NULL)
        {
            EQ_METHOD(0, void, 0x00464AB6, (void*), HWM_EQMAIN)();
            EQ_METHOD(0, void, 0x00464B15, (void*), HWM_EQMAIN)();
        }
        if (player == EQ_GLOBAL(EQPlayer*, 0x006E4DBC))
            EQ_METHOD(1, void, 0x004C53D8, (void*, int), g_everquest_guess)(0);
        if (player == EQ_GLOBAL(EQPlayer*, 0x006E4DC0) && HWM_EQMAIN != NULL &&
            HWM_FIELD(int, HWM_EQMAIN, 0x685DC) != 0)
        {
            EQ_METHOD(0, void, 0x00464C8D, (void*), HWM_EQMAIN)();
            EQ_METHOD(0, void, 0x00464CD1, (void*), HWM_EQMAIN)();
        }
        if (player == EQ_GLOBAL(EQPlayer*, 0x006E4DC4))
        {
            if (HWM_EQMAIN != NULL && HWM_EQMAIN[0xFE0] == 6)
            {
                if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && EQ_GLOBAL(void*, 0x006ECDF4) != NULL)
                    EQ_METHOD(1, void, 0x004E4EE6, (void*, int), EQ_GLOBAL(void*, 0x006ECDF4))(0x15);
                duelMessage[0] = EQ_GLOBAL(EQPlayer*, 0x006E4DC4)->spawn_id;
                duelMessage[1] = g_local_player_guess->spawn_id;
                HWM_SEND(0x209D, duelMessage, 8, 1);
                ((HWMWindow_guess*)HWM_EQMAIN)->Deactivate_guess();
                HWM_EQMAIN[0xFE0] = HWM_EQMAIN[0xFE1];
                ((HWMWindow_guess*)HWM_EQMAIN)->Activate_guess();
                EQ_GLOBAL(u32, 0x006B7120) = 0x14;
                EQ_METHOD(2, void, 0x004CE8C0, (void*, int, int), g_everquest_guess)(0, 1);
                if (HWM_EQMAIN != NULL)
                {
                    if (HWM_FIELD(int, HWM_EQMAIN, 0xE5C) > 0)
                        HWM_FIELD(int, HWM_EQMAIN, 0xE5C)--;
                    else
                        HWM_FIELD(int, HWM_EQMAIN, 0xE5C) = 0;
                }
                if (HWM_FIELD(int, HWM_EQMAIN, 0xE5C) != 0)
                {
                    errorMessage.unknown0x00 = -4;
                    errorMessage.unknown0x04 = HWM_FIELD(int, HWM_EQMAIN, 0xE5C);
                    errorMessage.reason = 2;
                    HWM_SEND(0x2147, &errorMessage, sizeof(MsgClientError), 1);
                }
                HWM_FIELD(int, HWM_EQMAIN, 0xE5C) = 0;
                if (EQ_GLOBAL(int, 0x00590170) == 2)
                {
                    g_game_state_guess = 2;
                    if (HWM_EQMAIN != NULL)
                        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
                    EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
                }
            }
            EQ_GLOBAL(EQPlayer*, 0x006E4DC4) = NULL;
        }
        if (EQ_GLOBAL(short, 0x006B6772) != -1 && EQ_GLOBAL(EQPlayer*, 0x006B6780) == player)
        {
            EQ_GLOBAL(short, 0x006B6772) = -1;
            EQ_GLOBAL(EQPlayer*, 0x006B6780) = NULL;
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), g_everquest_guess)(EQ_ARRAY(char, 0x0055F920));
        }
        if (player == EQ_GLOBAL(EQPlayer*, 0x006E4DAC))
        {
            g_attacking_guess = 0;
            g_rangedAttacking_guess = 0;
            EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = NULL;
            if (HWM_EQMAIN == NULL)
                goto spawn_unlinked;
            HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
            if (HWM_EQMAIN[0x100C] != 0)
            {
                ((HWMWindow_guess*)HWM_EQMAIN)->Deactivate_guess();
                HWM_EQMAIN[0xFE0] = HWM_EQMAIN[0xFE1];
                ((HWMWindow_guess*)HWM_EQMAIN)->Activate_guess();
            }
        }
        if (HWM_EQMAIN == NULL)
            goto spawn_unlinked;
        if (player == HWM_FIELD(EQPlayer*, HWM_EQMAIN, 0x84))
        {
            if (EQ_GLOBAL(int, 0x00590170) == 2)
            {
                g_game_state_guess = 2;
                if (g_display_guess != NULL)
                    EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
                HWM_EQMAIN[0xFE1] = 0;
            }
            ((HWMWindow_guess*)HWM_EQMAIN)->Deactivate_guess();
            HWM_EQMAIN[0xFE0] = HWM_EQMAIN[0xFE1];
            ((HWMWindow_guess*)HWM_EQMAIN)->Activate_guess();
        }
        for (slot = 0; slot < 1000; slot++)
        {
            if (HWM_FIELD(EQPlayer*, HWM_EQMAIN, 0x685F8 + slot * 0x10) == player)
            {
                HWM_FIELD(EQPlayer*, HWM_EQMAIN, 0x685F8 + slot * 0x10) = NULL;
                HWM_FIELD(float, HWM_EQMAIN, 0x685FC + slot * 0x10) = EQ_GLOBAL(float, 0x005345BC);
                HWM_FIELD(int, HWM_EQMAIN, (slot + 0x6860) * 0x10) = -1;
                HWM_FIELD(int, HWM_EQMAIN, 0x68604 + slot * 0x10) = 0;
                break;
            }
        }
    spawn_unlinked:
        if (player == g_local_player_guess)
            break;
        if (EQ_METHOD(0, int, 0x00408E7F, (void*), g_display_guess)() != 0)
        {
            if (player->race == 0xF4)
            {
                sprintf(text, EQ_ARRAY(char, 0x0055F904), player->name);
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), g_everquest_guess)(text);
            }
            else if (player->race >= 0xF6)
            {
                sprintf(text, EQ_ARRAY(char, 0x0055F8E4), player->name);
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), g_everquest_guess)(text);
            }
        }
        EQ_METHOD(1, void*, 0x004A00A1, (EQPlayer*, int), player)(1);
        break;
    case 0x2127:
        if (g_pc_guess == NULL)
            break;
        if (g_pc_guess->guild_id != *(int*)data)
            break;
        if (*(int*)(data + 4) != 0)
            break;
        g_pc_guess->guild_id = -1;
        g_pc_guess->guild_rank = 0;
        HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F898), 0xF);
        goto guild_changed;
    case 0x2134:
        DbgLog_guess(EQ_ARRAY(char, 0x0055F87C));
        break;
    case 0x2138:
        if (g_pc_guess == NULL)
            break;
        EQ_METHOD(3, int, 0x00480E87, (void*, int, int, int), g_pc_guess)(
            *(int*)data, *(int*)(data + 4), *(int*)(data + 8));
        break;
    case 0x2132:
        if (g_pc_guess == NULL)
            break;
        for (slot = 0; slot < 15; slot++)
        {
            if (g_pc_guess->buffs[slot].visible != 0 && g_pc_guess->buffs[slot].visible == data[4] &&
                g_pc_guess->buffs[slot].spell_id == *(short*)(data + 8))
                EQ_METHOD(2, void, 0x00424E30, (void*, SpellBuff*, int), g_pc_guess)(&g_pc_guess->buffs[slot], 0);
        }
        break;
    case 0x2152:
        if (g_pc_guess == NULL)
            break;
        g_pc_guess->hunger = (s8)data[4];
        g_pc_guess->thirst = (s8)data[5];
        if (g_pc_guess->hunger < 0)
            g_pc_guess->hunger = 0;
        if (g_pc_guess->thirst < 0)
            g_pc_guess->thirst = 0;
        if (data[6] == 0)
            break;
        EQ_METHOD(1, void, 0x004C5026, (void*, u32), this)(*(u32*)data);
        break;
    case 0x2146:
        if (EQ_CDECL(int, 0x00491DD7, (int, int, int))(*(int*)data, *(int*)(data + 4), *(int*)(data + 8)) != 0)
            break;
        HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F84C), 0xF);
        break;
    case 0x212C:
        if (EQ_CDECL(int, 0x00492102, (int, int, int))(*(int*)data, *(int*)(data + 4), *(int*)(data + 8)) != 0)
            break;
        HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F824), 0xF);
        break;
    case 0x2137:
        *(u32*)data = g_local_player_guess->spawn_id;
        if (*(u32*)data == 0)
            break;
        player = HWM_GET_PLAYER_BY_ID(*(int*)(data + 4));
        if (player == NULL)
            break;
        EQ_METHOD(2, void, 0x004C8EBE, (void*, EQPlayer*, u8*), this)(player, data);
        break;
    case 0x212F:
        EQ_CDECL(void, 0x0049231B, (void*, u32, u8*, u32))(connection, opcode, data, size);
        break;
    case 0x212A:
        EQ_CDECL(void, 0x00495C57, (u8*))(data);
        break;
    case 0x2136:
        EQ_GLOBAL(u32, 0x006B70E0) = 1;
        EQ_GLOBAL(u32, 0x006B6510) = *(u32*)data;
        EQ_GLOBAL(u32, 0x006B6514) = *(u32*)(data + 4);
        if (g_pc_guess == NULL)
        {
            EQ_GLOBAL(u32, 0x006B70E0) = 0;
            break;
        }
        for (slot = 0; slot < 30; slot++)
        {
            EQ_ARRAY(short, 0x006B6FC0)[slot] = -1;
            item = (u8*)g_pc_guess->inventory_items[slot];
            if (item != NULL)
            {
                if (item[0x81] == 1)
                {
                    bagItems = &EQ_ARRAY(short, 0x006B6FFC)[(short)(slot == 0 ? 8 : slot - 0x16) * 10];
                    for (index = 0; index < 10; index++)
                        bagItems[index] = -1;
                    for (index = 0; index < item[0xD5]; index++)
                    {
                        object = HWM_FIELD(u8*, item, 0xAC + index * 4);
                        if (object != NULL)
                            bagItems[index] = HWM_FIELD(short, object, 0x82);
                    }
                }
                EQ_ARRAY(short, 0x006B6FC0)[slot] = g_pc_guess->inventory_items[slot]->item_nr;
            }
            EQ_GLOBAL(s32, 0x006B70B0) = g_pc_guess->platinum_bank;
            EQ_GLOBAL(s32, 0x006B70B4) = g_pc_guess->gold_bank;
            EQ_GLOBAL(s32, 0x006B70B8) = g_pc_guess->silver_bank;
            EQ_GLOBAL(s32, 0x006B70BC) = g_pc_guess->copper_bank;
            EQ_GLOBAL(s32, 0x006B70C0) = g_pc_guess->platinum;
            EQ_GLOBAL(s32, 0x006B70C4) = g_pc_guess->gold;
            EQ_GLOBAL(s32, 0x006B70C8) = g_pc_guess->silver;
            EQ_GLOBAL(s32, 0x006B70CC) = g_pc_guess->copper;
            EQ_GLOBAL(s32, 0x006B70D0) = g_pc_guess->platinum_cursor;
            EQ_GLOBAL(s32, 0x006B70D4) = g_pc_guess->gold_cursor;
            EQ_GLOBAL(s32, 0x006B70D8) = g_pc_guess->silver_cursor;
            EQ_GLOBAL(s32, 0x006B70DC) = g_pc_guess->copper_cursor;
        }
        break;
    case 0x2149:
        EQ_CDECL(void, 0x0049286E, (void*, u32, u8*, u32))(connection, opcode, data, size);
        break;
    case 0x212B:
        if (*(int*)(data + 0x44) != 0)
        {
            if (strcmp(g_local_player_guess->name, (const char*)data + 0x24) == 0)
                break;
            if (*(int*)(data + 0x44) != 0)
            {
                HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F800), 0xF);
                break;
            }
        }
        HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F7DC), 0xF);
        break;
    case 0x2130:
        if (g_pc_guess == NULL)
            break;
        if (*(int*)(data + 0x3C) != 0)
            break;
        if (*(int*)(data + 0x40) != 0)
            break;
        strcpy((char*)data, (const char*)data + 0x1E);
        strcpy((char*)data + 0x1E, g_pc_guess->name);
        *(u32*)(data + 0x3C) = g_pc_guess->unknown0x103C;
        *(u32*)(data + 0x40) = g_pc_guess->unknown0x1040;
        HWM_SEND(0x2130, data, 0x44, 1);
        break;
    case 0x2133:
        switch (*(short*)(data + 0x60))
        {
        case 0:
            sprintf(echoText, EQ_ARRAY(char, 0x0055F784), data + 0x40, data + 0x62);
            break;
        case 2:
            sprintf(echoText, EQ_ARRAY(char, 0x0055F79C), data + 0x40, data + 0x62);
            break;
        case 3:
            sprintf(echoText, EQ_ARRAY(char, 0x0055F7B4), data + 0x40, data + 0x62);
            break;
        case 4:
            sprintf(echoText, EQ_ARRAY(char, 0x0055F7C8), data + 0x40, data + 0x62);
            break;
        case 5:
            sprintf(echoText, EQ_ARRAY(char, 0x0055F764), data + 0x40, data + 0x62);
            break;
        case 7:
        case 14:
            sprintf(echoText, EQ_ARRAY(char, 0x0055F740), data + 0x40, data + 0x20, data + 0x62);
            break;
        default:
            sprintf(echoText, EQ_ARRAY(char, 0x0055F754), data + 0x40, data + 0x62);
            break;
        }
        HWM_DSP_CHAT(echoText, 0xE);
        break;
    case 0x214F:
        if (g_pc_guess == NULL)
            break;
        if (_strcmpi(g_pc_guess->name, (const char*)data) == 0 && strlen((const char*)data + 0x20) != 0 &&
            _strcmpi(EQ_ARRAY(char, 0x006E1DE8), (const char*)data + 0x20) == 0)
        {
            if (*(int*)(data + 0x50) != 0)
            {
                HWM_FIELD(int, g_local_player_guess->actor_info, 0x18C) = 0;
                g_pc_guess->boat[0] = 0;
                g_local_player_guess->y = *(float*)(data + 0x40);
                g_local_player_guess->x = *(float*)(data + 0x44);
                g_local_player_guess->z = *(float*)(data + 0x48);
                g_local_player_guess->heading = *(float*)(data + 0x4C);
                g_local_player_guess->pitch = 0.0f;
                g_local_player_guess->dy = 0.0f;
                g_local_player_guess->dx = 0.0f;
                g_local_player_guess->dz = 0.0f;
                g_local_player_guess->delta_heading = 0.0f;
                g_local_player_guess->unk_0x5C = 0.0f;
                g_local_player_guess->speed = 0.0f;
                EQ_GLOBAL(u8, 0x006E1DE7) = 1;
                break;
            }
            EQ_GLOBAL(u8, 0x006E1DE7) = 2;
        }
        else
        {
            EQ_GLOBAL(u8, 0x006E1DE7) = 0;
        }
        EQ_ARRAY(char, 0x006E1DE8)[0] = 0;
        break;
    case 0x2157:
        if (g_pc_guess == NULL)
            break;
        g_pc_guess->hunger = *(short*)data;
        g_pc_guess->thirst = *(short*)(data + 2);
        EQ_METHOD(1, void, 0x00480DFB, (void*, int), g_pc_guess)((s8)data[4]);
        if (*(short*)data > 32000)
            g_pc_guess->hunger = 32000;
        if (*(short*)(data + 2) > 32000)
            g_pc_guess->thirst = 32000;
        if (*(short*)data < 0)
            g_pc_guess->hunger = 0;
        if (*(short*)(data + 2) < 0)
            g_pc_guess->thirst = 0;
        if (HWM_EQMAIN == NULL)
            break;
        goto eqmain_dirty;
    case 0x218E:
        HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4DF4), 0x9C) = 0;
        if (*(int*)data == -1)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F6F0), 0x12);
            break;
        }
        memcpy(EQ_GLOBAL(u8*, 0x006E4DF4) + 0x118, data, size);
        HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4DF4), 0xA0) = 0;
        strcpy(EQ_GLOBAL(char*, 0x006E4DF4) + 0x9BC, EQ_GLOBAL(char*, 0x006E4DF4) + 0x5BC);
        HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4DF4), 0xA4) = 0;
        HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4DF4), 0xA8) = 0;
        HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4DF4), 0xAC) = 0;
        HWM_WINDOW(0x006E4DF4)->Deactivate_guess();
        HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4DF4), 0x8C) = 2;
        HWM_WINDOW(0x006E4DF4)->Activate_guess();
        sprintf(text, EQ_ARRAY(char, 0x00558F88), EQ_GLOBAL(char*, 0x006E4DF4) + 0x1BC);
        HWM_DSP_CHAT(text, 0x12);
        break;
    case 0x2189:
        EQ_CDECL(void, 0x0049553F, (u8*))(data);
        break;
    case 0x2183:
        EQ_CDECL(void, 0x004954F6, (u8*))(data);
        break;
    case 0x2199:
        if (g_pc_guess == NULL)
            break;
        g_pc_guess->exp = *(u32*)data;
        goto eqmain_dirty;
    case 0x2198:
        EQ_CDECL(void, 0x0049494E, (void*, u32, u8*, u32))(connection, opcode, data, size);
        break;
    case 0x2190:
        if (g_pc_guess == NULL)
            break;
        EQ_METHOD(2, void, 0x00427EE7, (void*, short, short), g_pc_guess)(*(short*)data, *(short*)(data + 4));
        break;
    case 0x2182:
        EQ_CDECL(void, 0x00491E6E, (int, int, int))(*(int*)(data + 8), *(int*)data, *(int*)(data + 4));
        break;
    case 0x215B:
        if (g_pc_guess == NULL)
            break;
        EQ_METHOD(1, void, 0x004246DD, (void*, int), g_pc_guess)(*(int*)data);
        break;
    case 0x217F:
        if (g_pc_guess == NULL)
            break;
        g_pc_guess->mana = *(short*)data;
        object = (u8*)g_local_player_guess->actor_info;
        if (object[0x24A] < 8)
        {
            HWM_FIELD(int, object, 0xD4 + object[0x24A] * 4) = HWM_FIELD(int, g_display_guess, 0xBC);
            HWM_FIELD(int, g_local_player_guess->actor_info, 0x15C) = HWM_FIELD(int, g_display_guess, 0xBC);
        }
        if (size == 4)
        {
            EQ_METHOD(2, void, 0x004279B4, (void*, int, short), g_pc_guess)(3, *(short*)(data + 2));
            goto eqmain_dirty;
        }
        EQ_METHOD(1, void, 0x0042798F, (void*, int), g_pc_guess)(3);
        goto eqmain_dirty;
    case 0x217B:
        if (*(int*)data < 0)
            break;
        if (*(int*)data >= 0x200)
            break;
        memcpy(EQ_ARRAY(u8, 0x006E5490) + *(int*)data * 0x3C, data + 4, 0x3C);
        EQ_CDECL(void, 0x004DFF8B, (int))(-1);
    guild_changed:
        EQ_METHOD(1, void, 0x00411B7B, (void*, EQPlayer*), g_display_guess)(NULL);
        break;
    case 0x216E:
        player = HWM_GET_PLAYER((const char*)data);
        if (player == NULL)
            break;
        if (*(short*)(data + 0x5C) == 0)
            break;
        if (HWM_EQMAIN != NULL)
            HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        strcpy(player->last_name, (const char*)data + 0x3C);
        if (player->spawn_id == g_local_player_guess->spawn_id)
            strcpy(g_pc_guess->last_name, (const char*)data + 0x3C);
        EQ_METHOD(1, void, 0x00411B7B, (void*, EQPlayer*), g_display_guess)(player);
        break;
    case 0x2193:
        if (*(int*)(data + 0x44) == -5)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F650), 0x111);
            break;
        }
        if (*(int*)(data + 0x44) == -2)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055F68C), data + 0x1E);
            HWM_DSP_CHAT(text, 0x111);
            break;
        }
        if (*(int*)(data + 0x44) != -1)
            break;
        sprintf(text, EQ_ARRAY(char, 0x0055F6B0), data + 0x1E);
        HWM_DSP_CHAT(text, 0x111);
        break;
    case 0x216F:
        if (*(int*)(data + 0x44) == 0)
            break;
        if (g_pc_guess->guild_rank != 2)
            break;
        if (HWM_WINDOW(0x006E4E20) != NULL && HWM_IS_OPEN(HWM_WINDOW(0x006E4E20)) == 0)
        {
            HWM_FIELD(u8, HWM_WINDOW(0x006E4E20), 0x17C) = 8;
            HWM_WINDOW(0x006E4E20)->Activate_guess();
            HWM_FIELD(int, HWM_WINDOW(0x006E4E20), 0x170) = *(int*)(data + 0x40);
            HWM_FIELD(short, HWM_WINDOW(0x006E4E20), 0x176) = *(short*)(data + 0x3C);
            strcpy((char*)HWM_WINDOW(0x006E4E20) + 0x5B9, (const char*)data + 0x1E);
            sprintf((char*)HWM_WINDOW(0x006E4E20) + 0x1B9, EQ_ARRAY(char, 0x0055F5F0), data + 0x1E,
                EQ_ARRAY(char, 0x006E5494) + *(int*)(data + 0x3C) * 0x3C, *(int*)(data + 0x40));
        guild_request_timer:
            HWM_FIELD(int, HWM_WINDOW(0x006E4E20), 0x178) = HWM_FIELD(int, g_display_guess, 0xBC) + 10000;
            break;
        }
        strcpy((char*)guildReply, (const char*)data + 0x1E);
        strcpy((char*)guildReply + 0x1E, (const char*)data);
        *(int*)(guildReply + 0x44) = -1;
        *(int*)(guildReply + 0x3C) = *(int*)(data + 0x3C);
        *(int*)(guildReply + 0x40) = *(int*)(data + 0x40);
        HWM_SEND(0x2193, guildReply, 0x48, 1);
        break;
    case 0x2194:
        if (*(int*)(data + 0x44) == -5)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F548), 0x111);
            break;
        }
        if (*(int*)(data + 0x44) == -2)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055F588), data + 0x1E);
            HWM_DSP_CHAT(text, 0x111);
            break;
        }
        if (*(int*)(data + 0x44) != -1)
            break;
        sprintf(text, EQ_ARRAY(char, 0x0055F5B0), data + 0x1E);
        HWM_DSP_CHAT(text, 0x111);
        break;
    case 0x2191:
        if (*(int*)(data + 0x44) == 0)
            break;
        if (g_pc_guess->guild_rank != 2)
            break;
        if (HWM_WINDOW(0x006E4E20) != NULL && HWM_IS_OPEN(HWM_WINDOW(0x006E4E20)) == 0)
        {
            HWM_FIELD(u8, HWM_WINDOW(0x006E4E20), 0x17C) = 9;
            HWM_WINDOW(0x006E4E20)->Activate_guess();
            HWM_FIELD(int, HWM_WINDOW(0x006E4E20), 0x170) = *(int*)(data + 0x40);
            HWM_FIELD(short, HWM_WINDOW(0x006E4E20), 0x176) = *(short*)(data + 0x3C);
            strcpy((char*)HWM_WINDOW(0x006E4E20) + 0x5B9, (const char*)data + 0x1E);
            sprintf((char*)HWM_WINDOW(0x006E4E20) + 0x1B9, EQ_ARRAY(char, 0x0055F4F4), data + 0x1E,
                EQ_ARRAY(char, 0x006E5494) + *(int*)(data + 0x3C) * 0x3C);
            goto guild_request_timer;
        }
        strcpy((char*)guildReply, (const char*)data + 0x1E);
        strcpy((char*)guildReply + 0x1E, (const char*)data);
        *(int*)(guildReply + 0x44) = -1;
        *(int*)(guildReply + 0x3C) = *(int*)(data + 0x3C);
        *(int*)(guildReply + 0x40) = *(int*)(data + 0x40);
        HWM_SEND(0x2194, guildReply, 0x48, 1);
        break;
    case 0x218C:
        HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F4AC), 0xD);
        g_pc_guess->pk = 1;
        break;
    case 0x215A:
        EQ_METHOD(0, void, 0x004D03CE, (void*), this)();
        break;
    case 0x2192:
        if (size <= 4)
            break;
        EQ_GLOBAL(int, 0x00575FA8) = (size - 4) / 0x3C;
        if (EQ_GLOBAL(int, 0x00575FA8) > 0x200)
        {
            DbgLog_guess(EQ_ARRAY(char, 0x0055F484), EQ_GLOBAL(int, 0x00575FA8));
            EQ_GLOBAL(int, 0x00575FA8) = 0x200;
        }
        if (EQ_GLOBAL(int, 0x00575FA8) <= 0)
            break;
        memset(EQ_ARRAY(u8, 0x006E5490), 0, 0x7800);
        memcpy(EQ_ARRAY(u8, 0x006E5490), data + 4, EQ_GLOBAL(int, 0x00575FA8) * 0x3C);
        EQ_CDECL(void, 0x004DFF8B, (int))(-1);
        break;
    case 0x21A9:
        if (g_local_player_guess == NULL)
            break;
        if (g_local_player_guess->actor_info == NULL)
            break;
        HWM_FIELD(int, g_local_player_guess->actor_info, 0x2F4) = 1;
        break;
    case 0x21AA:
        if (g_local_player_guess == NULL)
            break;
        if (g_local_player_guess->actor_info == NULL)
            break;
        HWM_FIELD(int, g_local_player_guess->actor_info, 0x2F4) = 0;
        break;
    case 0x2161:
        EQ_CDECL(void, 0x00492ADA, (void*, u32, u8*, u32))(connection, opcode, data, size);
        break;
    case 0x21A7:
        sprintf(text, EQ_ARRAY(char, 0x0055F46C), data + 0x20, data + 0x40);
        HWM_DSP_CHAT(text, 0xF);
        break;
    case 0x216D:
        DbgLog_guess(EQ_ARRAY(char, 0x0055F44C));
        goto kicked_from_world;
    case 0x2164:
        newItem = (EQ_Item*)new EquipmentAllocation_guess;
        memcpy(newItem, data, 0x124);
        if (EQ_GLOBAL(int, 0x006B70E0) != 0 || EQ_GLOBAL(int, 0x006B64E8) != 0)
            goto item_refused_busy;
        if (((u8*)newItem)[0xC2] == 0xD || newItem->item_nr < 1000)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
            operator delete(newItem);
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            errorMessage.reason = 1;
            goto send_client_error;
        }
        if (HWM_FIELD(int, this, 4) == 5 && EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E2DC), newItem->item_nr);
            HWM_DSP_CHAT(text, 0xD);
            newItem->icon_nr = 500;
        }
        if (((u8*)newItem)[0xD8] != 0)
        {
            EQ_GLOBAL(EQ_Item*, 0x005DFBBC) = newItem;
            if (EQ_CDECL(int, 0x004A1D83, (EQ_Item*))(newItem) != 0 && ((u8*)newItem)[0xDA] == 0)
            {
                sprintf(text, EQ_ARRAY(char, 0x0055E704), newItem->item_nr);
                HWM_DSP_CHAT(text, 0xD);
                operator delete(newItem);
                errorMessage.unknown0x00 = -2;
                errorMessage.unknown0x04 = -2;
                errorMessage.reason = 2;
                goto send_client_error;
            }
        }
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        if (newItem->equip_slot < 30)
        {
            g_pc_guess->inventory_items[newItem->equip_slot] = newItem;
        }
        else if (newItem->equip_slot <= 0x154)
        {
            index = newItem->equip_slot - 30;
            slot = index / 10;
            if ((short)slot == 30)
                slot = 0;
            item = (u8*)g_pc_guess->inventory_items[(short)slot];
            if (item == NULL || item[0x81] != 1)
            {
                operator delete(newItem);
                errorMessage.unknown0x00 = -2;
                errorMessage.unknown0x04 = -2;
                errorMessage.reason = 3;
                goto send_client_error;
            }
            if (item == NULL || HWM_FIELD(EQ_Item*, item, 0xAC + (short)(index % 10) * 4) != NULL)
            {
                operator delete(newItem);
                errorMessage.unknown0x00 = -2;
                errorMessage.unknown0x04 = -2;
                errorMessage.reason = 4;
                goto send_client_error;
            }
            HWM_FIELD(EQ_Item*, item, 0xAC + (short)(index % 10) * 4) = newItem;
        }
        else if (newItem->equip_slot >= 2000 && newItem->equip_slot < 2009)
        {
            g_pc_guess->bank_items[(short)(newItem->equip_slot - 2000)] = newItem;
        }
        else if (newItem->equip_slot >= 2030)
        {
            index = (short)(newItem->equip_slot - 2000) - 30;
            item = (u8*)g_pc_guess->bank_items[(short)(index / 10)];
            if (item == NULL || item[0x81] != 1)
            {
                operator delete(newItem);
                errorMessage.reason = 5;
                errorMessage.unknown0x00 = -2;
                errorMessage.unknown0x04 = -2;
                goto send_client_error;
            }
            if (item == NULL || HWM_FIELD(EQ_Item*, item, 0xAC + (short)(index % 10) * 4) != NULL)
            {
                operator delete(newItem);
                errorMessage.reason = 6;
                errorMessage.unknown0x00 = -2;
                errorMessage.unknown0x04 = -2;
                goto send_client_error;
            }
            HWM_FIELD(EQ_Item*, item, 0xAC + (short)(index % 10) * 4) = newItem;
        }
        else
        {
            operator delete(newItem);
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            errorMessage.reason = 7;
            goto send_client_error;
        }
        EQ_CDECL(void, 0x004A2816, (CharacterProfile*))(g_pc_guess);
        EQ_CDECL(void, 0x004A204D, (CharacterProfile*))(g_pc_guess);
        if (g_pc_guess->inventory_items[0] == NULL)
            break;
        g_displayManager_guess[0x3C] = 1;
        break;
    case 0x2165:
        newItem = (EQ_Item*)new NoteAllocation_guess;
        memcpy(newItem, data, 0xD0);
        if (EQ_GLOBAL(int, 0x006B70E0) != 0 || EQ_GLOBAL(int, 0x006B64E8) != 0)
            goto item_refused_busy;
        if (newItem->item_nr == 0)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
            if (newItem != NULL)
            {
                EQ_METHOD(0, void, 0x0040101D, (void*), newItem)();
                operator delete(newItem);
            }
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            errorMessage.reason = 8;
            goto send_client_error;
        }
        if (HWM_FIELD(int, this, 4) == 5 && EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E6E8), newItem->item_nr);
            HWM_DSP_CHAT(text, 0xD);
            newItem->icon_nr = 500;
        }
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        if (newItem->equip_slot < 30)
        {
            g_pc_guess->inventory_items[newItem->equip_slot] = newItem;
        }
        else if (newItem->equip_slot <= 0x154)
        {
            index = newItem->equip_slot - 30;
            slot = index / 10;
            if ((short)slot == 30)
                slot = 0;
            item = (u8*)g_pc_guess->inventory_items[(short)slot];
            if (item == NULL || item[0x81] != 1)
            {
                if (newItem != NULL)
                {
                    EQ_METHOD(0, void, 0x0040101D, (void*), newItem)();
                    operator delete(newItem);
                }
                errorMessage.unknown0x00 = -2;
                errorMessage.unknown0x04 = -2;
                errorMessage.reason = 9;
                goto send_client_error;
            }
            if (item == NULL || HWM_FIELD(EQ_Item*, item, 0xAC + (short)(index % 10) * 4) != NULL)
            {
                if (newItem != NULL)
                    EQ_METHOD(1, void*, 0x004A0085, (void*, u8), newItem)(1);
                errorMessage.unknown0x00 = -2;
                errorMessage.unknown0x04 = -2;
                errorMessage.reason = 0xA;
                goto send_client_error;
            }
            HWM_FIELD(EQ_Item*, item, 0xAC + (short)(index % 10) * 4) = newItem;
        }
        else if (newItem->equip_slot >= 2000 && newItem->equip_slot < 2009)
        {
            g_pc_guess->bank_items[(short)(newItem->equip_slot - 2000)] = newItem;
        }
        else if (newItem->equip_slot >= 2030)
        {
            index = (short)(newItem->equip_slot - 2000) - 30;
            item = (u8*)g_pc_guess->bank_items[(short)(index / 10)];
            if (item == NULL || item[0x81] != 1)
            {
                if (newItem != NULL)
                    EQ_METHOD(1, void*, 0x004A0085, (void*, u8), newItem)(1);
                errorMessage.reason = 0xB;
                errorMessage.unknown0x00 = -2;
                errorMessage.unknown0x04 = -2;
                goto send_client_error;
            }
            if (item == NULL || HWM_FIELD(EQ_Item*, item, 0xAC + (short)(index % 10) * 4) != NULL)
            {
                if (newItem != NULL)
                    EQ_METHOD(1, void*, 0x004A0085, (void*, u8), newItem)(1);
                errorMessage.reason = 0xC;
                errorMessage.unknown0x00 = -2;
                errorMessage.unknown0x04 = -2;
                goto send_client_error;
            }
            HWM_FIELD(EQ_Item*, item, 0xAC + (short)(index % 10) * 4) = newItem;
        }
        else
        {
            if (newItem != NULL)
                EQ_METHOD(1, void*, 0x004A0085, (void*, u8), newItem)(1);
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            errorMessage.reason = 0xD;
            goto send_client_error;
        }
        goto summoned_note_done;
    case 0x2166:
        newItem = (EQ_Item*)new ContainerAllocation_guess;
        memcpy(newItem, data, 0xDC);
        if (EQ_GLOBAL(int, 0x006B70E0) != 0 || EQ_GLOBAL(int, 0x006B64E8) != 0)
            goto item_refused_busy;
        if (newItem->item_nr == 0)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
            if (newItem != NULL)
                EQ_METHOD(1, void*, 0x004A0069, (void*, u8), newItem)(1);
            errorMessage.reason = 0xE;
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            goto send_client_error;
        }
        if (HWM_FIELD(int, this, 4) == 5 && EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E2F8), newItem->item_nr);
            HWM_DSP_CHAT(text, 0xD);
            newItem->icon_nr = 500;
        }
        ((u8*)newItem)[0xD6] = 0;
        for (index = 0; index < 10; index++)
            HWM_FIELD(EQ_Item*, newItem, 0xAC + index * 4) = NULL;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        if (newItem->equip_slot < 30)
        {
            if (g_pc_guess->inventory_items[newItem->equip_slot] == NULL)
            {
                g_pc_guess->inventory_items[newItem->equip_slot] = newItem;
                goto trade_item_placed;
            }
            if (newItem != NULL)
                EQ_METHOD(1, void*, 0x004A0069, (void*, u8), newItem)(1);
            errorMessage.reason = 0xF;
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            goto send_client_error;
        }
        if (newItem->equip_slot >= 2000 && newItem->equip_slot < 2009)
        {
            if (g_pc_guess->bank_items[(short)(newItem->equip_slot - 2000)] == NULL)
            {
                g_pc_guess->bank_items[(short)(newItem->equip_slot - 2000)] = newItem;
            trade_item_placed:
                EQ_CDECL(void, 0x004A2816, (CharacterProfile*))(g_pc_guess);
                if (g_pc_guess->inventory_items[0] == NULL)
                    break;
                g_displayManager_guess[0x3C] = 1;
                break;
            }
            if (newItem != NULL)
                EQ_METHOD(1, void*, 0x004A0069, (void*, u8), newItem)(1);
            errorMessage.reason = 0x10;
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            goto send_client_error;
        }
        if (newItem != NULL)
            EQ_METHOD(1, void*, 0x004A0069, (void*, u8), newItem)(1);
        errorMessage.reason = 0x11;
        errorMessage.unknown0x00 = -2;
        errorMessage.unknown0x04 = -2;
        goto send_client_error;
    item_refused_busy:
        errorMessage.reason = 1000;
        errorMessage.unknown0x00 = -2;
        errorMessage.unknown0x04 = -2;
        if (EQ_GLOBAL(int, 0x006B64E8) != 0)
            errorMessage.reason = 2000;
    send_client_error:
        HWM_SEND(0x2147, &errorMessage, sizeof(MsgClientError), 1);
        break;
    case 0x2181:
        DbgLog_guess(EQ_ARRAY(char, 0x0055F414));
        break;
    case 0x219A:
        EQ_GLOBAL(s32, 0x006B6844) += *(s32*)data;
        EQ_GLOBAL(s32, 0x006B6848) += *(s32*)(data + 4);
        EQ_GLOBAL(s32, 0x006B684C) += *(s32*)(data + 8);
        EQ_GLOBAL(s32, 0x006B6850) += *(s32*)(data + 0xC);
        break;
    case 0x21AE:
        if (g_pc_guess == NULL)
            break;
        if (strcmp(g_pc_guess->name, (const char*)data) != 0)
            break;
        for (slot = 0; slot < 30; slot++)
        {
            item = (u8*)g_pc_guess->inventory_items[slot];
            if (item == NULL)
                continue;
            if (HWM_FIELD(u32, item, 0xA0) == *(u32*)(data + 0x20))
            {
                if (HWM_IS_OPEN(HWM_WINDOW(0x006E4E58)) != 0 &&
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E58), 0x174) == item)
                {
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E58), 0x174)[0xD6] = 0;
                    HWM_WINDOW(0x006E4E58)->Deactivate_guess();
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E58), 0x174) = NULL;
                }
                if (HWM_IS_OPEN(HWM_WINDOW(0x006E4E5C)) != 0 &&
                    HWM_FIELD(EQ_Item*, HWM_WINDOW(0x006E4E5C), 0x174) == g_pc_guess->inventory_items[slot])
                {
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E5C), 0x174)[0xD6] = 0;
                    HWM_WINDOW(0x006E4E5C)->Deactivate_guess();
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E5C), 0x174) = NULL;
                }
                window = HWM_WINDOW(0x006E4E60);
                if (window->isOpen_guess != 0 &&
                    window->inspectedItem_guess == g_pc_guess->inventory_items[slot])
                {
                    window->Deactivate_guess();
                    HWM_FIELD(EQ_Item*, HWM_WINDOW(0x006E4E60), 0x37AC) = NULL;
                }
                operator delete(g_pc_guess->inventory_items[slot]);
                g_pc_guess->inventory_items[slot] = NULL;
                if (slot == 0)
                    g_displayManager_guess[0x3C] = 0;
                strcpy((char*)deleteMessage, (const char*)data);
                *(int*)(deleteMessage + 0x20) = slot;
                EQ_METHOD(0, void, 0x004D56B0, (void*), g_everquest_guess)();
                HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
                EQ_CDECL(void, 0x004A204D, (CharacterProfile*))(g_pc_guess);
                HWM_SEND(0x21B2, deleteMessage, 0x24, 1);
                continue;
            }
            if (item[0x81] != 1)
                continue;
            for (index = 0; index < item[0xD5]; index++)
            {
                object = HWM_FIELD(u8*, item, 0xAC + index * 4);
                if (object == NULL || HWM_FIELD(u32, object, 0xA0) != *(u32*)(data + 0x20))
                    continue;
                window = HWM_WINDOW(0x006E4E60);
                if (window->isOpen_guess != 0 && HWM_FIELD(u8*, window, 0x37AC) == object)
                {
                    window->Deactivate_guess();
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E60), 0x37AC) = NULL;
                }
                operator delete(HWM_FIELD(u8*, item, 0xAC + index * 4));
                HWM_FIELD(u8*, item, 0xAC + index * 4) = NULL;
                strcpy((char*)deleteMessage, (const char*)data);
                *(int*)(deleteMessage + 0x20) = index + (slot * 5 + 0xF) * 2;
                if (slot == 0)
                    *(int*)(deleteMessage + 0x20) = index + 0x14A;
                HWM_SEND(0x21B2, deleteMessage, 0x24, 1);
                HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
            }
        }
        break;
    case 0x21CE:
        if (*(int*)data >= 0 && *(int*)(data + 4) >= 0)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F3F0), 0xF);
            savedRace = g_pc_guess->spell_book[*(int*)data];
            g_pc_guess->spell_book[*(int*)data] = g_pc_guess->spell_book[*(int*)(data + 4)];
            g_pc_guess->spell_book[*(int*)(data + 4)] = savedRace;
        }
        HWM_FIELD(int, HWM_EQMAIN, 0xED4) = -1;
    eqmain_dirty:
        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        break;
    case 0x21C4:
        if (*(int*)(data + 0x20) == 0)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F3B0), 0xF);
            break;
        }
        sprintf(text, EQ_ARRAY(char, 0x0055F37C), g_pc_guess, data + 0x24);
        HWM_DSP_CHAT(text, 0xF);
        break;
    case 0x21C3:
        EQ_GLOBAL(u32, 0x00596330) = *(u32*)(data + 4);
        EQ_GLOBAL(u32, 0x006B7108) = EQ_GLOBAL(u32, 0x00596330);
        break;
    case 0x21BA:
        EQ_CDECL(void, 0x004955B1, (u8*))(data);
        break;
    case 0x21B5:
        if (g_pc_guess == NULL)
            break;
        if (strcmp(g_pc_guess->name, (const char*)data) != 0)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        strcpy((char*)coinReply, (const char*)data);
        *(int*)(coinReply + 0x20) = *(int*)(data + 0x20);
        *(int*)(coinReply + 0x24) = *(int*)(data + 0x24);
        switch (*(int*)(data + 0x20))
        {
        case 1:
            coins = &g_pc_guess->platinum_bank;
            break;
        case 2:
            coins = &g_pc_guess->gold_bank;
            break;
        case 3:
            coins = &g_pc_guess->silver_bank;
            break;
        case 4:
            coins = &g_pc_guess->copper_bank;
            break;
        default:
            goto world_done;
        }
        if (*(int*)(data + 0x24) > *coins)
        {
            *(int*)(coinReply + 0x24) = *coins;
            *coins = 0;
        }
        else
        {
            *coins -= *(int*)(data + 0x24);
        }
        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        HWM_SEND(0x21B7, coinReply, 0x28, 1);
        break;
    case 0x21B4:
        if (g_pc_guess == NULL)
            break;
        if (strcmp(g_pc_guess->name, (const char*)data) != 0)
            break;
        for (slot = 0; slot < 8; slot++)
        {
            item = (u8*)g_pc_guess->bank_items[slot];
            if (item == NULL)
                continue;
            if (HWM_FIELD(u32, item, 0xA0) == *(u32*)(data + 0x20))
            {
                if (HWM_IS_OPEN(HWM_WINDOW(0x006E4E58)) != 0 &&
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E58), 0x174) == item)
                {
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E58), 0x174)[0xD6] = 0;
                    HWM_WINDOW(0x006E4E58)->Deactivate_guess();
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E58), 0x174) = NULL;
                }
                if (HWM_IS_OPEN(HWM_WINDOW(0x006E4E5C)) != 0 &&
                    HWM_FIELD(EQ_Item*, HWM_WINDOW(0x006E4E5C), 0x174) == g_pc_guess->bank_items[slot])
                {
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E5C), 0x174)[0xD6] = 0;
                    HWM_WINDOW(0x006E4E5C)->Deactivate_guess();
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E5C), 0x174) = NULL;
                }
                window = HWM_WINDOW(0x006E4E60);
                if (window->isOpen_guess != 0 && window->inspectedItem_guess == g_pc_guess->bank_items[slot])
                {
                    window->Deactivate_guess();
                    HWM_FIELD(EQ_Item*, HWM_WINDOW(0x006E4E60), 0x37AC) = NULL;
                }
                operator delete(g_pc_guess->bank_items[slot]);
                g_pc_guess->bank_items[slot] = NULL;
                if (slot == 0)
                    g_displayManager_guess[0x3C] = 0;
                strcpy((char*)deleteMessage, (const char*)data);
                *(int*)(deleteMessage + 0x20) = slot;
                HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
                HWM_SEND(0x21B6, deleteMessage, 0x24, 1);
                continue;
            }
            if (item[0x81] != 1)
                continue;
            for (index = 0; index < item[0xD5]; index++)
            {
                object = HWM_FIELD(u8*, item, 0xAC + index * 4);
                if (object == NULL || HWM_FIELD(u32, object, 0xA0) != *(u32*)(data + 0x20))
                    continue;
                window = HWM_WINDOW(0x006E4E60);
                if (window->isOpen_guess != 0 && HWM_FIELD(u8*, window, 0x37AC) == object)
                {
                    window->Deactivate_guess();
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E60), 0x37AC) = NULL;
                }
                operator delete(HWM_FIELD(u8*, item, 0xAC + index * 4));
                HWM_FIELD(u8*, item, 0xAC + index * 4) = NULL;
                strcpy((char*)deleteMessage, (const char*)data);
                *(int*)(deleteMessage + 0x20) = index + (slot * 5 + 0xF) * 2;
                HWM_SEND(0x21B6, deleteMessage, 0x24, 1);
                HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
            }
        }
        break;
    case 0x21B0:
        if (g_pc_guess == NULL)
            break;
        if (strcmp(g_pc_guess->name, (const char*)data) != 0)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        strcpy((char*)coinReply, (const char*)data);
        *(int*)(coinReply + 0x20) = *(int*)(data + 0x20);
        *(int*)(coinReply + 0x24) = *(int*)(data + 0x24);
        switch (*(int*)(data + 0x20))
        {
        case 1:
            coins = &g_pc_guess->platinum;
            break;
        case 2:
            coins = &g_pc_guess->gold;
            break;
        case 3:
            coins = &g_pc_guess->silver;
            break;
        case 4:
            coins = &g_pc_guess->copper;
            break;
        default:
            goto world_done;
        }
        if (*(int*)(data + 0x24) > *coins)
        {
            *(int*)(coinReply + 0x24) = *coins;
            *coins = 0;
        }
        else
        {
            *coins -= *(int*)(data + 0x24);
        }
        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        HWM_SEND(0x21B3, coinReply, 0x28, 1);
        break;
    case 0x21CF:
        HWM_WINDOW(0x006E4E20)->Deactivate_guess();
        HWM_FIELD(u8, HWM_WINDOW(0x006E4E20), 0x17C) = 1;
        HWM_WINDOW(0x006E4E20)->Activate_guess();
        HWM_FIELD(int, HWM_WINDOW(0x006E4E20), 0x178) = HWM_FIELD(int, g_display_guess, 0xBC) + 20000;
        sprintf((char*)HWM_WINDOW(0x006E4E20) + 0x1B9, EQ_ARRAY(char, 0x0055F31C));
        break;
    case 0x21EA:
        EQ_CDECL(void, 0x004958B9, (u8*))(data);
        break;
    case 0x21E2:
        HWM_FIELD(u32, g_display_guess, 0x14) = *(u32*)data;
        if (HWM_FIELD(u32, g_display_guess, 0x14) > 8)
            HWM_FIELD(u32, g_display_guess, 0x14) = 8;
        EQ_ARRAY(MsgNewZone, 0x006B65DC)->sky = HWM_FIELD(u8, g_display_guess, 0x14) + 1;
        EQ_METHOD(0, void, 0x004124DB, (void*), g_display_guess)();
        break;
    case 0x21DA:
        EQ_CDECL(void, 0x00495602, (u8*))(data);
        break;
    case 0x21D8:
        EQ_GLOBAL(int, 0x006E2058) = (s8)data[0] & 1;
        EQ_GLOBAL(u32, 0x006E205C) = ((u32)(s8)data[0] & 2) >> 1;
        EQ_GLOBAL(u32, 0x006E2060) = ((u32)(s8)data[0] & 4) >> 2;
        break;
    case 0x21D3:
        player = HWM_GET_PLAYER_BY_ID(*(int*)data);
        if (player == NULL)
            break;
        if (player->type != 0)
        {
            if (g_filterValue1_guess != 0)
                break;
            goto show_interrupt;
        }
        if (player->class_ == 8)
        {
            if (player == g_local_player_guess)
            {
                if (g_filterValue3_guess == 3)
                    break;
                goto show_interrupt;
            }
            for (slot = 0; slot < 5; slot++)
            {
                if ((u32)player == g_death_state_guess[slot])
                {
                    filter = g_filterValue3_guess;
                    goto check_group_filter;
                }
            }
            if (g_filterValue3_guess != 0)
                break;
            goto show_interrupt;
        }
        if (player == g_local_player_guess)
        {
            if (g_filterValue2_guess == 3)
                break;
            goto show_interrupt;
        }
        for (slot = 0; slot < 5; slot++)
        {
            if ((u32)player == g_death_state_guess[slot])
            {
                filter = g_filterValue2_guess;
                goto check_group_filter;
            }
        }
        if (g_filterValue2_guess != 0)
            break;
        goto show_interrupt;
    check_group_filter:
        if (filter != 0 && filter != 2)
            break;
    show_interrupt:
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)((const char*)data + 4, 0x108);
        break;
    case 0x21D0:
        if (HWM_IS_OPEN(EQ_GLOBAL(u8*, 0x006E4DF4)) != 0 && HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4DF4), 0x8C) >= 2)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F2A8), 0xF);
            break;
        }
        for (found = 0; found < 12; found++)
        {
            if (*(int*)(data + 0xA4 + found * 0x188) != 0)
                break;
        }
        if (found < 12 && found >= 0)
        {
            for (slot = 0xDC8; slot < 0x2028; slot += 0x188)
                memcpy(EQ_GLOBAL(u8*, 0x006E4DF4) + slot, data + slot - 0xDA8, 0x188);
            HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4DF4), 0x8C) = 4;
            HWM_FIELD(int, EQ_GLOBAL(u8*, 0x006E4DF4), 0xDBC) = found;
            HWM_WINDOW(0x006E4DF4)->Activate_guess();
            break;
        }
        HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F248), 0xF);
        break;
    case 0x21EC:
        EQ_CDECL(void, 0x00495D83, (u8*))(data);
        break;
    case 0x2205:
        if (HWM_EQMAIN == NULL)
            break;
        if (HWM_FIELD(u8*, HWM_EQMAIN, 0x1010) == NULL)
            break;
        for (slot = 0; slot < 10; slot++)
        {
            item = HWM_FIELD(u8*, HWM_FIELD(u8*, HWM_EQMAIN, 0x1010), 0xAC + slot * 4);
            if (item == NULL)
                continue;
            if (item[0x81] == 0)
            {
                operator delete(item);
                HWM_FIELD(u8*, HWM_FIELD(u8*, HWM_EQMAIN, 0x1010), 0xAC + slot * 4) = NULL;
            }
            else if (item[0x81] == 2)
            {
                if (item != NULL)
                {
                    EQ_METHOD(0, void, 0x0040101D, (void*), item)();
                    operator delete(item);
                }
                HWM_FIELD(u8*, HWM_FIELD(u8*, HWM_EQMAIN, 0x1010), 0xAC + slot * 4) = NULL;
            }
        }
        break;
    case 0x21F9:
        EQ_CDECL(void, 0x004960AE, (u8*))(data);
        break;
    case 0x21F4:
        EQ_CDECL(void, 0x00496060, (u8*))(data);
        break;
    case 0x21F2:
        if (g_local_player_guess == NULL)
            break;
        if (g_local_player_guess->actor_info == NULL)
            break;
        HWM_FIELD(int, g_local_player_guess->actor_info, 0x2DC) = *(int*)data;
        break;
    case 0x21F0:
        EQ_CDECL(void, 0x0049602C, (u8*))(data);
        break;
    case 0x2206:
        EQ_CDECL(void, 0x00495996, (u8*))(data);
        break;
    case 0x21F7:
        decompressed = EQ_CDECL(u32, 0x0049100C, (u8*, u8*, u32, u32))(data + 2, EQ_ARRAY(u8, 0x005F0248), size - 2, 0xC000);
        if (decompressed != (u32)(*(short*)data * 0x30))
        {
            *(short*)data = (short)(decompressed / 0x30);
            break;
        }
        for (index = 0; index < *(short*)data; index++)
        {
            object = (u8*)new EQSwitchAllocation_guess(0, 0, EQ_ARRAY(u8, 0x005F0248) + index * 0x30, 0);
            if (object != NULL && HWM_FIELD(int, object, 0x80) == 0 && object[0x1C] != 0x35)
            {
                EQ_METHOD(0, void, 0x004BDC26, (void*), object)();
                operator delete(object);
            }
        }
        break;
    case 0x2200:
        player = HWM_GET_PLAYER_BY_ID(*(int*)data);
        if (player == NULL)
            break;
        if (player == g_local_player_guess)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F22C), 0x111);
            break;
        }
        EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = player;
        if (EQ_GLOBAL(int, 0x006E206C) != 1)
            break;
        g_attacking_guess = 1;
        g_rangedAttacking_guess = 1;
        break;
    case 0x21FE:
        player = HWM_GET_PLAYER_BY_ID(*(int*)data);
        if (player == NULL)
            break;
        EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = player;
        break;
    case 0x21F6:
        EQ_METHOD(2, void, 0x00495456, (void*, u8*, u32), this)(data, size);
        break;
    case 0x4026:
        if (g_pc_guess == NULL)
            break;
        EQ_CDECL(void, 0x00491632, (u8*))(data);
        break;
    case 0x8004:
        DbgLog_guess(EQ_ARRAY(char, 0x0055F218));
        memcpy(zoneServer, data, 0x82);
        EQ_STDCALL(u16, 0x00500380, (u16))(*(u16*)(zoneServer + 0x80));
        EQ_CDECL(void, 0x004E0601, (void*))(EQ_GLOBAL(void*, 0x006B3194));
        EQ_GLOBAL(void*, 0x006B3194) = NULL;
        connectResult = EQ_CDECL(int, 0x004E055C, (void**, u8*, u16))(
            g_world_connection_guess, zoneServer, EQ_STDCALL(u16, 0x00500380, (u16))(*(u16*)(zoneServer + 0x80)));
        if (connectResult == 0)
        {
            *EQ_CDECL(int*, 0x004F23C0, (void*))(EQ_GLOBAL(void*, 0x006B3194)) = 0x300;
            EQ_CDECL(int, 0x004F46A0, (void*, int))(EQ_GLOBAL(void*, 0x006B3194), 0x1400);
            EQ_CDECL(void, 0x004F23A0, (void*, int))(EQ_GLOBAL(void*, 0x006B3194), 0x40000);
            EQ_GLOBAL(u32, 0x006B63D0) = 1;
            DbgLog_guess(EQ_ARRAY(char, 0x0055F208));
            break;
        }
        EQ_GLOBAL(u32, 0x006B63D0) = 0;
        DbgLog_guess(
            EQ_ARRAY(char, 0x0055F1B4), zoneServer, *(short*)(zoneServer + 0x80), connectResult);
        EQ_METHOD(1, void, 0x004D0FE6, (void*, const char*), this)(EQ_ARRAY(char, 0x0055F1A8));
        EQ_GLOBAL(HWMSleepFunction_guess, 0x00534198)(2000);
        break;
    case 0x8005:
        EQ_GLOBAL(u32, 0x006B61B4) = 1;
        EQ_GLOBAL(u32, 0x006B6490) = 0;
        break;
    }
world_done:
    if (EQ_METHOD(0, int, 0x00408E7F, (void*), g_display_guess)() == 0)
        return true;
    switch (opcode)
    {
    case 0x2015:
        EQ_METHOD(2, void, 0x004DBD4B, (void*, u8*, char*), g_everquest_guess)(data, firstWord);
        if (EQ_METHOD(1, int, 0x004CFF2D, (void*, char*), g_everquest_guess)(firstWord) != 0)
            return true;
        EQ_CDECL(void, 0x004AF87F, (u8*, char*, int, int, int))(
            data, firstWord, 0, 100, EQ_GLOBAL(int, 0x006B68B4));
        strcpy((char*)data, firstWord);
        color = 0x107;
    case 0x2014:
        parseDone = 0;
        parsePos = 0;
        EQ_CDECL(int, 0x004911D3, (u8*))(data);
        do
        {
            if (EQ_CDECL(int, 0x00491137, (u8*, char*, u32*, int))(data, line, &parsePos, 0x800) != 0)
            {
                HWM_DSP_CHAT(line, color);
            }
            else
            {
                if (strlen(line) != 0)
                    HWM_DSP_CHAT(line, color);
                parseDone = 1;
            }
        } while (parseDone == 0);
        break;
    case 0x2017:
        player = HWM_GET_PLAYER_BY_ID(*(int*)data);
        if (player == NULL)
            break;
        if (HWM_EQMAIN == NULL)
            break;
        if (EQ_GLOBAL(int, 0x006B698C) == 0)
            break;
        sprintf(text, EQ_ARRAY(char, 0x0055F18C), player->name, data + 4);
        HWM_DSP_CHAT(text, 0x102);
        break;
    case 0x2041:
        EQ_METHOD(1, void, 0x004CAEFB, (void*, u8*), this)(data);
        break;
    case 0x2040:
        EQ_METHOD(1, void, 0x004CB248, (void*, u8*), this)(data);
        break;
    case 0x2042:
        EQ_METHOD(1, void, 0x004CAEC9, (void*, u8*), this)(data);
        break;
    case 0x203F:
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        memcpy(&moneyTrade, data, sizeof(MsgMoneyTrade));
        if (moneyTrade.platinum > 0)
            sprintf(text, EQ_ARRAY(char, 0x0055F168), moneyTrade.platinum, HWM_TRIM_NAME(moneyTrade.name));
        else if (moneyTrade.gold > 0)
            sprintf(text, EQ_ARRAY(char, 0x0055F148), moneyTrade.gold, HWM_TRIM_NAME(moneyTrade.name));
        else if (moneyTrade.silver > 0)
            sprintf(text, EQ_ARRAY(char, 0x0055F128), moneyTrade.silver, HWM_TRIM_NAME(moneyTrade.name));
        else if (moneyTrade.copper > 0)
            sprintf(text, EQ_ARRAY(char, 0x0055F108), moneyTrade.copper, HWM_TRIM_NAME(moneyTrade.name));
        HWM_DSP_CHAT(text, 2);
        g_pc_guess->platinum += moneyTrade.platinum;
        g_pc_guess->gold += moneyTrade.gold;
        g_pc_guess->silver += moneyTrade.silver;
        g_pc_guess->copper += moneyTrade.copper;
        break;
    case 0x203D:
        memcpy(summonReply, data, 0x29);
        if (summonReply[0x28] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055F0F0), HWM_TRIM_NAME((const char*)summonReply));
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), g_everquest_guess)(text);
            break;
        }
        EQ_METHOD(1, void, 0x004CF46A, (void*, u8*), this)(summonReply);
        break;
    case 0x203C:
        if (g_pc_guess->inventory_items[0] == NULL)
        {
            summonReply[0x28] = 1;
        }
        else
        {
            summonReply[0x28] = 0;
            sprintf(text, EQ_ARRAY(char, 0x0055F0B4), HWM_TRIM_NAME((const char*)data));
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), g_everquest_guess)(text);
        }
        strcpy((char*)summonReply, (const char*)data);
        HWM_SEND(0x203D, summonReply, 0x29, 1);
        break;
    case 0x202C:
        if (data[0x98] >= 2)
            break;
        player = HWM_GET_PLAYER((const char*)data + 1);
        if (player == NULL)
            break;
        savedLink64 = player->previous_bits_guess;
        savedNext = player->next;
        savedActor = player->actor_info;
        savedCharacter = player->char_info_guess;
        savedTop = HWM_PLAYER_TOP;
        *player = *(EQPlayer*)data;
        HWM_PLAYER_TOP = savedTop;
        player->previous_bits_guess = savedLink64;
        player->next = savedNext;
        player->actor_info = savedActor;
        player->char_info_guess = savedCharacter;
        if (HWM_FIELD(int, this, 4) != 5)
            break;
        HWM_UPDATE_ACTOR(player);
        break;
    case 0x2092:
        player = HWM_GET_PLAYER_BY_ID(*(int*)data);
        if (player == NULL)
            break;
        player->weaponAnimationType = data[0xC];
        if (data[4] == 7 || data[4] == 8)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055F0AC), data[5]);
            EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, u8, char*), g_display_guess)(player, data[4], text);
            break;
        }
        if (player->texture != 0xFF)
            break;
        if (data[4] == 0)
        {
            EQ_METHOD(6, void, 0x00407334, (void*, EQPlayer*, int, int, int, int, int), g_display_guess)(
                player, data[5], player->equipment[0], 0, *(int*)(data + 8), 0);
            break;
        }
        if (data[4] == 9)
        {
            EQ_METHOD(3, void, 0x00406EDC, (void*, EQPlayer*, int, int), g_display_guess)(player, data[5], player->face);
            break;
        }
        EQ_METHOD(5, void, 0x0040798E, (void*, EQPlayer*, int, int, int, int), g_display_guess)(
            player, data[4], data[5], player->equipment[data[4]], *(int*)(data + 8));
        break;
    case 0x2007:
        object = (u8*)new MoneyListAllocation_guess;
        savedLink0 = HWM_FIELD(u32, object, 0);
        savedTop = EQ_GLOBAL(EQPlayer*, 0x006E4D60);
        savedLink4 = HWM_FIELD(u32, object, 4);
        if (object == NULL)
            goto allocation_failed;
        *(MsgDropCoins*)object = *(MsgDropCoins*)data;
        EQ_GLOBAL(EQPlayer*, 0x006E4D60) = savedTop;
        HWM_FIELD(u32, object, 0) = savedLink0;
        HWM_FIELD(u32, object, 4) = savedLink4;
        floor = HWM_FLOOR_HEIGHT_AT(HWM_FIELD(float, object, 0x38), HWM_FIELD(float, object, 0x3C),
                    HWM_FIELD(float, object, 0x40) + EQ_GLOBAL(float, 0x005344A4)) +
                EQ_GLOBAL(float, 0x005344A8);
        HWM_FIELD(int, object, 0x34) = HWM_CREATE_ACTOR((char*)object + 0x50, HWM_FIELD(float, object, 0x38),
            HWM_FIELD(float, object, 0x3C), floor, HWM_FIELD(float, object, 0x44));
        break;
    case 0x2035:
        object = (u8*)new ItemListAllocation_guess;
        savedLink0 = HWM_FIELD(u32, object, 0);
        savedTop = EQ_GLOBAL(EQPlayer*, 0x006E4D5C);
        savedLink4 = HWM_FIELD(u32, object, 4);
        if (object == NULL)
            goto allocation_failed;
        memcpy(object, data, 0xF0);
        EQ_GLOBAL(EQPlayer*, 0x006E4D5C) = savedTop;
        HWM_FIELD(u32, object, 0) = savedLink0;
        HWM_FIELD(u32, object, 4) = savedLink4;
        floor = HWM_FLOOR_HEIGHT_AT(HWM_FIELD(float, object, 0x28), HWM_FIELD(float, object, 0x2C),
                    HWM_FIELD(float, object, 0x30) + EQ_GLOBAL(float, 0x005344A4)) +
                EQ_GLOBAL(float, 0x005344A8);
        HWM_FIELD(int, object, 0x24) = HWM_CREATE_ACTOR((char*)object + 0x38, HWM_FIELD(float, object, 0x28),
            HWM_FIELD(float, object, 0x2C), floor, HWM_FIELD(float, object, 0x34));
        break;
    case 0x2008:
        if (g_local_player_guess == NULL)
            break;
        if (*(u32*)(data + 4) != 0 && *(u32*)(data + 4) == g_local_player_guess->spawn_id && HWM_EQMAIN != NULL)
        {
            HWM_COUNT_DOWN(HWM_FIELD(int, HWM_EQMAIN, 0x94));
            HWM_LOG_LOOT_EVENT(9);
        }
        object = EQ_METHOD(1, u8*, 0x004ADA11, (void*, int), EQ_GLOBAL(void*, 0x006E4D60))(*(int*)data);
        if (object == NULL)
            break;
        if (*(u32*)(data + 4) != 0 && *(u32*)(data + 4) == g_local_player_guess->spawn_id)
        {
            window = HWM_WINDOW(0x006018C0);
            if (window != NULL && window->isOpen_guess != 0)
                window->Deactivate_guess();
            window = HWM_WINDOW(0x00604984);
            if (window != NULL && window->isOpen_guess != 0)
            {
                window->forcedClose_guess = 1;
                HWM_WINDOW(0x00604984)->Deactivate_guess();
            }
            item = (u8*)g_displayManager_guess;
            if (item[0x3D] == 0 && g_pc_guess->platinum_cursor == 0 && g_pc_guess->gold_cursor == 0 &&
                g_pc_guess->silver_cursor == 0 && g_pc_guess->copper_cursor == 0)
            {
                item[0x3D] = 0;
                if (HWM_FIELD(int, object, 8) != 0)
                    g_pc_guess->platinum_cursor = HWM_FIELD(int, object, 8);
                else if (HWM_FIELD(int, object, 0xC) != 0)
                    g_pc_guess->gold_cursor = HWM_FIELD(int, object, 0xC);
                else if (HWM_FIELD(int, object, 0x10) != 0)
                    g_pc_guess->silver_cursor = HWM_FIELD(int, object, 0x10);
                else if (HWM_FIELD(int, object, 0x14) != 0)
                    g_pc_guess->copper_cursor = HWM_FIELD(int, object, 0x14);
                if (g_pc_guess->platinum_cursor > 0)
                    g_displayManager_guess[0x3D] = 1;
                else if (g_pc_guess->gold_cursor > 0)
                    g_displayManager_guess[0x3D] = 2;
                else if (g_pc_guess->silver_cursor > 0)
                    g_displayManager_guess[0x3D] = 3;
                else if (g_pc_guess->copper_cursor > 0)
                    g_displayManager_guess[0x3D] = 4;
                if (g_game_state_guess == 2 &&
                    (g_pcloudFlagA_guess == 0 || EQ_GLOBAL(int, 0x006B63FC) == 0x280))
                {
                    EQ_GLOBAL(int, 0x00590170) = 2;
                    g_game_state_guess = 1;
                    if (HWM_EQMAIN != NULL)
                        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
                    EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
                }
                if (HWM_EQMAIN != NULL)
                {
                    ((HWMWindow_guess*)HWM_EQMAIN)->Deactivate_guess();
                    HWM_EQMAIN[0xFE0] = 4;
                    ((HWMWindow_guess*)HWM_EQMAIN)->Activate_guess();
                }
            }
        }
        EQ_METHOD(1, void*, 0x004A00BD, (void*, u8), object)(1);
        break;
    case 0x2036:
        if (g_local_player_guess == NULL)
            break;
        if (*(u32*)(data + 4) == 0 || *(u32*)(data + 4) == g_local_player_guess->spawn_id)
        {
            HWM_LOG_LOOT_EVENT(8);
            HWM_COUNT_DOWN(HWM_FIELD(int, HWM_EQMAIN, 0x94));
            HWM_COUNT_DOWN(HWM_FIELD(int, HWM_EQMAIN, 0xE5C));
            HWM_LOG_LOOT_EVENT(0xA);
        }
        object = EQ_METHOD(1, u8*, 0x004A03F7, (void*, int), EQ_GLOBAL(void*, 0x006E4D5C))(*(int*)data);
        if (object == NULL)
            break;
        if (HWM_FIELD(int, object, 0xE4) < 0)
            break;
        EQ_METHOD(1, void*, 0x004A00D9, (void*, u8), object)(1);
        break;
    case 0x207E:
        if (HWM_EQMAIN == NULL)
            break;
        HWM_COUNT_DOWN(HWM_FIELD(int, HWM_EQMAIN, 0x94));
        HWM_LOG_LOOT_EVENT(0xE);
        break;
    case 0x207F:
        if (HWM_EQMAIN == NULL)
            break;
        HWM_COUNT_DOWN(HWM_FIELD(int, HWM_EQMAIN, 0x94));
        HWM_LOG_LOOT_EVENT(0xF);
        break;
    case 0x205E:
        if (HWM_EQMAIN == NULL)
            break;
        HWM_COUNT_DOWN(HWM_FIELD(int, HWM_EQMAIN, 0x94));
        HWM_LOG_LOOT_EVENT(0xB);
        break;
    case 0x2038:
        if (HWM_EQMAIN == NULL)
            break;
        if ((s8)data[8] < 0)
            break;
        item = HWM_FIELD(u8*, HWM_EQMAIN, 0x6C5E0 + (s8)data[8] * 4);
        if (item == NULL)
            break;
        if (HWM_FIELD(u8**, HWM_EQMAIN, 0x6C658) != NULL && *HWM_FIELD(u8**, HWM_EQMAIN, 0x6C658) == item)
        {
            HWM_FIELD(u8**, HWM_EQMAIN, 0x6C658) = NULL;
            HWM_FIELD(short, HWM_EQMAIN, 0x6C65C) = -1;
        }
        operator delete(HWM_FIELD(u8*, HWM_EQMAIN, 0x6C5E0 + (s8)data[8] * 4));
        HWM_FIELD(u8*, HWM_EQMAIN, 0x6C5E0 + (s8)data[8] * 4) = NULL;
        goto zone_eqmain_dirty;
    case 0x2028:
        EQ_CDECL(void, 0x00492D9B, (void*, u32, u8*, u32))(connection, opcode, data, size);
        break;
    case 0x2037:
        if (HWM_EQMAIN == NULL)
            break;
        EQ_METHOD(0, void, 0x00464AB6, (void*), HWM_EQMAIN)();
        break;
    case 0x2027:
        if (HWM_EQMAIN == NULL)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        HWM_COUNT_DOWN(HWM_FIELD(int, HWM_EQMAIN, 0x94));
        HWM_LOG_LOOT_EVENT(0xD);
        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        if (data != NULL && *(short*)(data + 8) >= 0 &&
            HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x6C5E0 + *(short*)(data + 8) * 4) != NULL)
        {
            if (EQ_CDECL(int, 0x004A406D, (CharacterProfile*, int))(g_pc_guess, *(int*)(data + 0x10)) == 0)
            {
                g_pc_guess->copper = 0;
                g_pc_guess->silver = 0;
                g_pc_guess->gold = 0;
                g_pc_guess->platinum = 0;
            }
            else if (g_sound_manager_ptr_guess != NULL)
            {
                HWM_WAVE_PLAY(0x8A);
            }
            sprintf(text, EQ_ARRAY(char, 0x0055F098),
                EQ_METHOD(2, char*, 0x00438701, (EQ_Item*, float, int),
                    HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x6C5E0 + *(short*)(data + 8) * 4))(
                    HWM_FIELD(float, HWM_EQMAIN, 0x1018), *(int*)(data + 0xC)),
                EQ_GLOBAL(EQPlayer*, 0x006E4DB8) == NULL ? EQ_ARRAY(char, 0x0055E2A4)
                                                         : HWM_TRIM_NAME(EQ_GLOBAL(EQPlayer*, 0x006E4DB8)->name));
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), g_everquest_guess)(text);
        }
        else if (*(short*)(data + 8) >= 0)
        {
            if (HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x6C5E0 + *(short*)(data + 8) * 4) != NULL)
            {
                operator delete(HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x6C5E0 + *(short*)(data + 8) * 4));
                HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x6C5E0 + *(short*)(data + 8) * 4) = NULL;
            }
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), g_everquest_guess)(EQ_ARRAY(char, 0x0055F060));
        }
        else
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055F018), 0xD);
        }
        HWM_FIELD(int, HWM_EQMAIN, 0xA0) = 1;
        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        break;
    case 0x203E:
        switch (*(int*)(data + 0x40))
        {
        case 0:
            receivedItem = (EQ_Item*)new EquipmentAllocation_guess;
            newItem = receivedItem;
            memcpy(newItem, data + 0x44, 0x124);
            if (((u8*)newItem)[0xC2] != 0xD)
                goto received_item_image;
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
            operator delete(newItem);
            break;
        case 2:
            receivedItem = (EQ_Item*)new NoteAllocation_guess;
            newItem = receivedItem;
            memcpy(newItem, data + 0x44, 0xD0);
        received_item_image:
            if (EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
            {
                sprintf(text, EQ_ARRAY(char, 0x0055E2DC), newItem->item_nr);
                HWM_DSP_CHAT(text, 0xD);
                newItem->icon_nr = 500;
            }
            receivedItem = newItem;
            break;
        }
        sprintf(text, EQ_ARRAY(char, 0x0055EFFC), receivedItem, data);
        HWM_DSP_CHAT(text, 4);
        if (HWM_EQMAIN == NULL)
            break;
        goto zone_eqmain_dirty;
    case 0x200C:
        EQ_CDECL(void, 0x004937C3, (void*, u32, u8*, u32))(connection, opcode, data, size);
        break;
    case 0x200D:
        newItem = (EQ_Item*)new ContainerAllocation_guess;
        memcpy(newItem, data, 0xDC);
        ((u8*)newItem)[0xD6] = 0;
        for (index = 0; index < 10; index++)
            HWM_FIELD(EQ_Item*, newItem, 0xAC + index * 4) = NULL;
        if (HWM_EQMAIN == NULL)
            break;
        if (HWM_FIELD(int, HWM_EQMAIN, 0x685E0) == 0)
            break;
        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x6C5E0 + newItem->equip_slot * 4) = newItem;
        break;
    case 0x2029:
        newItem = (EQ_Item*)new NoteAllocation_guess;
        memcpy(newItem, data, 0xD0);
        if (HWM_EQMAIN == NULL)
            break;
        if (HWM_FIELD(int, HWM_EQMAIN, 0x685E0) == 0)
            break;
        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x6C5E0 + newItem->equip_slot * 4) = newItem;
        break;
    case 0x200B:
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DB8) != NULL)
            break;
        player = HWM_GET_PLAYER_BY_ID(*(int*)data);
        if (player == NULL)
            break;
        if (HWM_EQMAIN == NULL)
            break;
        if (data[8] == 0)
        {
            HWM_COUNT_DOWN(HWM_FIELD(int, HWM_EQMAIN, 0xE5C));
            if (HWM_FIELD(int, HWM_EQMAIN, 0xE5C) != 0)
            {
                errorMessage.unknown0x00 = -4;
                errorMessage.unknown0x04 = HWM_FIELD(int, HWM_EQMAIN, 0xE5C);
                errorMessage.reason = 0xA;
                HWM_SEND(0x2147, &errorMessage, sizeof(MsgClientError), 1);
            }
            HWM_FIELD(int, HWM_EQMAIN, 0xE5C) = 0;
            break;
        }
        if (g_game_state_guess == 2 && (g_pcloudFlagA_guess == 0 || EQ_GLOBAL(int, 0x006B63FC) == 0x280))
        {
            EQ_GLOBAL(int, 0x00590170) = 2;
            g_game_state_guess = 1;
            HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        }
        EQ_GLOBAL(EQPlayer*, 0x006E4DB8) = player;
        HWM_FIELD(int, HWM_EQMAIN, 0x1018) = *(int*)(data + 0xC);
        EQ_METHOD(1, void, 0x0046498F, (void*, u8), HWM_EQMAIN)(data[8]);
        break;
    case 0x2053:
        newItem = (EQ_Item*)new ContainerAllocation_guess;
        memcpy(newItem, data, 0xDC);
        if (newItem->item_nr == 0)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
            if (newItem == NULL)
                break;
            EQ_METHOD(1, void*, 0x004A0069, (void*, u8), newItem)(1);
            break;
        }
        ((u8*)newItem)[0xD6] = 0;
        for (index = 0; index < 10; index++)
            HWM_FIELD(EQ_Item*, newItem, 0xAC + index * 4) = NULL;
        if (EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E2F8), newItem->item_nr);
            HWM_DSP_CHAT(text, 0xD);
            newItem->icon_nr = 500;
        }
        if (HWM_EQMAIN == NULL)
            break;
        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        ((u8*)newItem)[0xD6] = 0;
        for (slot = 0; slot < 30; slot++)
        {
            if (HWM_FIELD(short, HWM_EQMAIN, 0x6C8B6 + slot * 2) == -1)
                break;
        }
        HWM_FIELD(short, HWM_EQMAIN, 0x6C8B6 + slot * 2) = newItem->equip_slot;
        HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x6C5E0 + slot * 4) = newItem;
        break;
    case 0x2056:
        newItem = (EQ_Item*)new NoteAllocation_guess;
        memcpy(newItem, data, 0xD0);
        if (newItem->item_nr == 0)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
            if (newItem == NULL)
                break;
            EQ_METHOD(1, void*, 0x004A0085, (void*, u8), newItem)(1);
            break;
        }
        if (EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E6E8), newItem->item_nr);
            HWM_DSP_CHAT(text, 0xD);
            newItem->icon_nr = 500;
        }
        goto place_loot_item;
    case 0x2052:
        newItem = (EQ_Item*)new EquipmentAllocation_guess;
        memcpy(newItem, data, 0x124);
        if (newItem->item_nr == 0 || ((u8*)newItem)[0xC2] == 0xD)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
            operator delete(newItem);
            break;
        }
        if (EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E2DC), newItem->item_nr);
            HWM_DSP_CHAT(text, 0xD);
            newItem->icon_nr = 500;
        }
    place_loot_item:
        if (HWM_EQMAIN == NULL)
            break;
        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        for (slot = 0; slot < 30; slot++)
        {
            if (HWM_FIELD(short, HWM_EQMAIN, 0x6C8B6 + slot * 2) == -1)
                break;
        }
        if (newItem->equip_slot < 30)
        {
            HWM_FIELD(short, HWM_EQMAIN, 0x6C8B6 + slot * 2) = newItem->equip_slot;
            HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x6C5E0 + slot * 4) = newItem;
            break;
        }
        if (newItem->equip_slot >= 400)
            break;
        index = (short)(newItem->equip_slot - 30);
        for (found = 0; found < 30; found++)
        {
            if (HWM_FIELD(short, HWM_EQMAIN, 0x6C8B6 + found * 2) == (short)(index / 10))
                break;
        }
        object = HWM_FIELD(u8*, HWM_EQMAIN, 0x6C5E0 + found * 4);
        if (object == NULL)
            break;
        HWM_FIELD(EQ_Item*, object, 0xAC + (short)(index % 10) * 4) = newItem;
        break;
    case 0x204F:
    case 0x2144:
        if (HWM_EQMAIN == NULL)
            break;
        EQ_METHOD(0, void, 0x00464CD1, (void*), HWM_EQMAIN)();
        break;
    case 0x2050:
        if (HWM_EQMAIN == NULL)
            break;
        if (EQ_GLOBAL(void*, 0x006E4DC0) == NULL)
            break;
        for (slot = 0; slot < 30; slot++)
            HWM_FIELD(short, HWM_EQMAIN, 0x6C8B6 + slot * 2) = -1;
        EQ_METHOD(5, void, 0x00464E5B, (void*, u8, int, int, int, int), HWM_EQMAIN)(
            data[0], *(int*)(data + 4), *(int*)(data + 8), *(int*)(data + 0xC), *(int*)(data + 0x10));
        break;
    case 0x2045:
        new MissileAllocation_guess(0, 0, data, 0, 0, 0);
        break;
    case 0x2046:
        memcpy(&castOn, data, sizeof(MsgCastOn));
        player = HWM_GET_PLAYER_BY_ID(castOn.target_id);
        if (player == NULL)
            break;
        EQ_METHOD(1, void, 0x004B3C9D, (EQPlayer*, MsgCastOn*), player)(&castOn);
        break;
    case 0x2048:
        memcpy(soundMessage, data, 8);
        if (*(short*)(soundMessage + 4) < 0)
            break;
        if (*(short*)(soundMessage + 4) >= 0x2D0)
            break;
        player = HWM_GET_PLAYER_BY_ID(*(int*)soundMessage);
        if (player == NULL)
            break;
        EQ_METHOD(5, void, 0x0040E3EA, (void*, int, int, float, float, float), g_display_guess)(
            0xFF, *(short*)(soundMessage + 4), player->y, player->x, player->z);
        break;
    case 0x204A:
        memcpy(&death, data, sizeof(MsgDeath));
        EQ_METHOD(1, void, 0x004C555F, (void*, MsgDeath*), this)(&death);
        break;
    case 0x204C:
        player = HWM_GET_PLAYER((const char*)data);
        if (player == NULL)
            break;
        if (player == EQ_GLOBAL(EQPlayer*, 0x006E4DC0) && HWM_EQMAIN != NULL &&
            HWM_FIELD(int, HWM_EQMAIN, 0x685DC) != 0)
            EQ_METHOD(0, void, 0x00464C8D, (void*), HWM_EQMAIN)();
        EQ_METHOD(1, void*, 0x004A00A1, (EQPlayer*, int), player)(1);
        break;
    case 0x2058:
        EQ_CDECL(void, 0x004949F1, (u8*))(data);
        break;
    case 0x209C:
        EQ_CDECL(void, 0x00493581, (void*, u32, u8*, u32))(connection, opcode, data, size);
        break;
    case 0x20AC:
        player = HWM_GET_PLAYER_BY_ID(*(int*)data);
        if (player == NULL)
            break;
        if (data[8] != 0)
        {
            killer = HWM_GET_PLAYER_BY_ID(*(int*)(data + 4));
            if (killer == NULL)
                break;
            sprintf(text, EQ_ARRAY(char, 0x0055EFE0), player->name, killer->name);
        }
        else
        {
            sprintf(text, EQ_ARRAY(char, 0x0055EFC0), player->name);
        }
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), g_everquest_guess)(text);
        break;
    case 0x20AA:
        EQ_CDECL(void, 0x00493FE8, (void*, u32, u8*, u32))(connection, 0x20AA, data, size);
        break;
    case 0x20A1:
        updateCount = (size - 4) / 0xF;
        if (updateCount > *(u32*)data)
        {
            EQ_CDECL(int, 0x005229E8, (const char*, u32, u32))(EQ_ARRAY(char, 0x0055EF98), *(u32*)data, updateCount);
            *(u32*)data = updateCount;
        }
        for (index = 0; (u32)index < *(u32*)data; index++)
            EQ_CDECL(void, 0x00491075, (u8*))(data + 4 + index * 0xF);
        break;
    case 0x209F:
        memcpy(animation, data, 0xC);
        player = HWM_GET_PLAYER_BY_ID(animation[0]);
        if (player == NULL)
            break;
        EQ_METHOD(6, void, 0x0048ED3B, (void*, EQPlayer*, int, int, int, float, int), g_display_guess)(
            player, animation[1], 1, 0, *(float*)&animation[2], 1);
        break;
    case 0x20AD:
        EQ_CDECL(void, 0x00494485, (void*, u32, u8*, u32))(connection, opcode, data, size);
        break;
    case 0x20D8:
        EQ_GLOBAL(int, 0x006B63E8) = 1;
        if (EQ_GLOBAL(int, 0x006E2068) == 1)
            EQ_CDECL(void, 0x004A8B39, (int, void*))(0, EQ_ARRAY(void, 0x0055B254));
        if (EQ_GLOBAL(int, 0x006E2064) != 1)
            break;
        EQ_CDECL(void, 0x004A8A32, (int, void*))(0, EQ_ARRAY(void, 0x0055B250));
        break;
    case 0x20D6:
        player = HWM_GET_PLAYER_BY_ID(*(int*)(data + 4));
        if (player == NULL)
            break;
        if (data[8] == 0x62)
            sprintf(text, EQ_ARRAY(char, 0x0055EF54), player->name);
        else if (data[8] == 0x63)
            sprintf(text, EQ_ARRAY(char, 0x0055EF08), player->name);
        else
            sprintf(text, EQ_ARRAY(char, 0x0055EEE4), player->name);
        HWM_DSP_CHAT(text, 0x111);
        break;
    case 0x20D1:
        player = HWM_GET_PLAYER_BY_ID(*(int*)(data + 4));
        tradeReply[8] = 0;
        if (player == NULL)
            break;
        if (EQ_GLOBAL(int, 0x006B6868) == 0)
        {
            tradeReply[8] = 0x62;
            goto trade_busy;
        }
        if (EQ_GLOBAL(int, 0x006B6868) == 1)
        {
            tradeReply[8] = 0x63;
            if (g_name_buffer_guess[0] == 0)
                goto trade_busy;
            for (slot = 0; slot < 5; slot++)
            {
                if ((u32)player == g_death_state_guess[slot])
                    tradeReply[8] = 0;
            }
            if (tradeReply[8] != 0)
                goto trade_busy;
        }
        if (HWM_EQMAIN == NULL || (s8)HWM_EQMAIN[0xFE0] >= 5 ||
            HWM_FIELD(int, g_local_player_guess->actor_info, 0x68) != 0 || EQ_GLOBAL(int, 0x006B6590) != 0 ||
            HWM_FIELD(int, HWM_EQMAIN, 0xE5C) != 0 || HWM_FIELD(int, HWM_EQMAIN, 0x94) != 0 || g_attacking_guess != 0)
        {
            tradeReply[8] = 1;
        trade_busy:
            *(int*)(tradeReply + 0) = *(int*)(data + 4);
            *(u32*)(tradeReply + 4) = g_local_player_guess->spawn_id;
            HWM_SEND(0x20D6, tradeReply, 0xC, 1);
        trade_interest:
            sprintf(text, EQ_ARRAY(char, 0x0055EEC0), player->name);
            HWM_DSP_CHAT(text, 0x111);
            break;
        }
        if (g_game_state_guess == 2)
        {
            g_game_state_guess = 1;
            HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        }
        ((HWMWindow_guess*)HWM_EQMAIN)->Deactivate_guess();
        HWM_EQMAIN[0xFE1] = HWM_EQMAIN[0xFE0];
        EQ_GLOBAL(EQPlayer*, 0x006E4D88) = player;
        HWM_EQMAIN[0xFE0] = 4;
        HWM_FIELD(int, HWM_EQMAIN, 0x685E4) = 1;
        HWM_FIELD(int, HWM_EQMAIN, 0x6C908) = 1;
        ((HWMWindow_guess*)HWM_EQMAIN)->Activate_guess();
        EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = player;
        duelMessage[0] = player->spawn_id;
        duelMessage[1] = g_local_player_guess->spawn_id;
        HWM_SEND(0x20E6, duelMessage, 8, 1);
        HWM_FIELD(int, HWM_EQMAIN, 0xE5C)++;
        if (HWM_FIELD(int, HWM_EQMAIN, 0xE5C) != 1)
        {
            errorMessage.unknown0x00 = -4;
            errorMessage.unknown0x04 = HWM_FIELD(int, HWM_EQMAIN, 0xE5C);
            errorMessage.reason = 0x10;
            HWM_SEND(0x2147, &errorMessage, sizeof(MsgClientError), 1);
        }
        HWM_FIELD(int, HWM_EQMAIN, 0xE5C) = 1;
        break;
    case 0x20B2:
        player = HWM_GET_PLAYER_BY_ID(*(int*)data);
        if (player == NULL)
            break;
        if (player == g_local_player_guess)
        {
            player->cur_hp = *(int*)(data + 4);
            if (g_pc_guess == NULL)
                break;
            g_pc_guess->cur_hp = *(short*)(data + 4);
            break;
        }
        if (player->type == 1 && *(int*)(data + 4) <= 100 && *(int*)(data + 8) == 100)
        {
            player->cur_hp = *(int*)(data + 4);
        }
        else if (*(int*)(data + 4) <= 0)
        {
            if (*(int*)(data + 4) >= -0x7E)
                player->cur_hp = *(int*)(data + 4);
            else
                player->cur_hp = -0x7E;
        }
        else
        {
            player->cur_hp = *(int*)(data + 4) * 100 / *(int*)(data + 8);
            if (player->cur_hp <= 0)
                player->cur_hp = 1;
        }
        player->max_hp = 100;
        break;
    case 0x20DA:
        if (HWM_EQMAIN == NULL)
            break;
        if (HWM_FIELD(int, HWM_EQMAIN, 0x685E4) == 0)
            break;
        HWM_FIELD(int, HWM_EQMAIN, 0x685F0) = data[4];
        break;
    case 0x20E4:
        if (HWM_EQMAIN == NULL)
            break;
        if (HWM_FIELD(int, HWM_EQMAIN, 0x685E4) == 0)
            break;
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D88) == NULL)
            break;
        HWM_FIELD(int, HWM_EQMAIN, (0x1B152 - data[4]) * 4) += *(int*)(data + 8);
        HWM_FIELD(int, HWM_EQMAIN, 0x685F0) = 0;
        HWM_FIELD(int, HWM_EQMAIN, 0x685EC) = 0;
        goto zone_eqmain_dirty;
    case 0x20DF:
        if (HWM_EQMAIN == NULL)
            break;
        if (HWM_FIELD(int, HWM_EQMAIN, 0x685E4) == 0)
            break;
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D88) == NULL)
            break;
        HWM_FIELD(int, HWM_EQMAIN, 0x685EC) = 0;
        HWM_FIELD(int, HWM_EQMAIN, 0x685F0) = 0;
        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        if (*(short*)(data + 4) < 0)
            break;
        if (*(short*)(data + 4) < 8)
        {
            item = HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4);
            if (item != NULL && item[0x81] == 1)
            {
                if (HWM_IS_OPEN(HWM_WINDOW(0x006E4E58)) != 0 && item == HWM_FIELD(u8*, HWM_WINDOW(0x006E4E58), 0x174))
                {
                    item[0xD6] = 0;
                    HWM_WINDOW(0x006E4E58)->Deactivate_guess();
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E58), 0x174) = NULL;
                }
                else if (HWM_IS_OPEN(HWM_WINDOW(0x006E4E5C)) != 0 &&
                         item == HWM_FIELD(u8*, HWM_WINDOW(0x006E4E5C), 0x174))
                {
                    item[0xD6] = 0;
                    HWM_WINDOW(0x006E4E5C)->Deactivate_guess();
                    HWM_FIELD(u8*, HWM_WINDOW(0x006E4E5C), 0x174) = NULL;
                }
            }
            switch (data[6])
            {
            case 0:
                newItem = (EQ_Item*)new EquipmentAllocation_guess;
                memcpy(newItem, data + 7, 0x124);
                if (newItem->item_nr == 0 || ((u8*)newItem)[0xC2] == 0xD)
                {
                    HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
                    operator delete(newItem);
                    break;
                }
                if (EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
                {
                    sprintf(text, EQ_ARRAY(char, 0x0055E2DC), newItem->item_nr);
                    HWM_DSP_CHAT(text, 0xD);
                    newItem->icon_nr = 500;
                }
                item = HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4);
                if (item != NULL && EQ_CDECL(int, 0x004A1D83, (u8*))(item) != 0 &&
                    newItem->item_nr == HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4)->item_nr)
                {
                    stackCount = &HWM_FIELD(u8, HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4), 0xDA);
                    goto add_stack_charges;
                }
                operator delete(HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4));
                HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4) = NULL;
                HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4) = newItem;
                break;
            case 1:
                newItem = (EQ_Item*)new ContainerAllocation_guess;
                memcpy(newItem, data + 7, 0xDC);
                if (newItem->item_nr == 0)
                {
                    HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
                    if (newItem == NULL)
                        break;
                    EQ_METHOD(1, void*, 0x004A0069, (void*, u8), newItem)(1);
                    break;
                }
                ((u8*)newItem)[0xD6] = 0;
                for (index = 0; index < 10; index++)
                    HWM_FIELD(EQ_Item*, newItem, 0xAC + index * 4) = NULL;
                if (EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
                {
                    sprintf(text, EQ_ARRAY(char, 0x0055E2F8), newItem->item_nr);
                    HWM_DSP_CHAT(text, 0xD);
                    newItem->icon_nr = 500;
                }
                if (HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4) != NULL)
                {
                    operator delete(HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4));
                    HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4) = NULL;
                }
                HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4) = newItem;
                break;
            case 2:
                newItem = (EQ_Item*)new NoteAllocation_guess;
                memcpy(newItem, data + 7, 0xD0);
                if (newItem->item_nr == 0)
                {
                    HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
                    if (newItem == NULL)
                        break;
                    EQ_METHOD(1, void*, 0x004A0085, (void*, u8), newItem)(1);
                    break;
                }
                if (EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
                {
                    sprintf(text, EQ_ARRAY(char, 0x0055E6E8), newItem->item_nr);
                    HWM_DSP_CHAT(text, 0xD);
                    newItem->icon_nr = 500;
                }
                if (HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4) != NULL)
                {
                    operator delete(HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4));
                    HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4) = NULL;
                }
                HWM_FIELD(EQ_Item*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4) = newItem;
                break;
            case 0xFF:
                if (HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4) == NULL)
                    break;
                operator delete(HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4));
                HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) * 4) = NULL;
                break;
            }
            break;
        }
        if (*(short*)(data + 4) < 30)
            break;
        *(short*)(data + 4) -= 30;
        object = HWM_FIELD(u8*, HWM_EQMAIN, 0x6C4EC + *(short*)(data + 4) / 10 * 4);
        if (object == NULL)
            break;
        if (object[0x81] != 1)
            break;
        switch (data[6])
        {
        case 0xFF:
            if (HWM_FIELD(u8*, object, 0xAC + *(short*)(data + 4) % 10 * 4) == NULL)
                break;
            operator delete(HWM_FIELD(u8*, object, 0xAC + *(short*)(data + 4) % 10 * 4));
            HWM_FIELD(u8*, object, 0xAC + *(short*)(data + 4) % 10 * 4) = NULL;
            break;
        case 2:
            newItem = (EQ_Item*)new NoteAllocation_guess;
            memcpy(newItem, data + 7, 0xD0);
            if (newItem->item_nr == 0)
            {
                HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
                if (newItem == NULL)
                    break;
                EQ_METHOD(1, void*, 0x004A0085, (void*, u8), newItem)(1);
                break;
            }
        replace_bag_slot:
            if (HWM_FIELD(u8*, object, 0xAC + *(short*)(data + 4) % 10 * 4) != NULL)
            {
                operator delete(HWM_FIELD(u8*, object, 0xAC + *(short*)(data + 4) % 10 * 4));
                HWM_FIELD(u8*, object, 0xAC + *(short*)(data + 4) % 10 * 4) = NULL;
            }
            HWM_FIELD(EQ_Item*, object, 0xAC + *(short*)(data + 4) % 10 * 4) = newItem;
            break;
        case 0:
            newItem = (EQ_Item*)new EquipmentAllocation_guess;
            memcpy(newItem, data + 7, 0x124);
            if (newItem->item_nr == 0)
            {
                HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
                operator delete(newItem);
                break;
            }
            item = HWM_FIELD(u8*, object, 0xAC + *(short*)(data + 4) % 10 * 4);
            if (item == NULL || EQ_CDECL(int, 0x004A1D83, (u8*))(item) == 0 ||
                newItem->item_nr != HWM_FIELD(EQ_Item*, object, 0xAC + *(short*)(data + 4) % 10 * 4)->item_nr)
            {
                operator delete(HWM_FIELD(u8*, object, 0xAC + *(short*)(data + 4) % 10 * 4));
                HWM_FIELD(u8*, object, 0xAC + *(short*)(data + 4) % 10 * 4) = NULL;
                HWM_FIELD(EQ_Item*, object, 0xAC + *(short*)(data + 4) % 10 * 4) = newItem;
                break;
            }
            stackCount = &HWM_FIELD(u8, HWM_FIELD(u8*, object, 0xAC + *(short*)(data + 4) % 10 * 4), 0xDA);
        add_stack_charges:
            *stackCount += ((u8*)newItem)[0xDA];
            if (*stackCount > 0x14)
                *stackCount = 0x14;
            break;
        }
        break;
    case 0x20DC:
        if (HWM_EQMAIN == NULL)
            break;
        if (HWM_FIELD(int, HWM_EQMAIN, 0x685E4) == 0)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        EQ_METHOD(0, void, 0x004573D2, (void*), HWM_EQMAIN)();
        break;
    case 0x20DB:
        if (HWM_EQMAIN == NULL)
            break;
        if (HWM_FIELD(int, HWM_EQMAIN, 0x685E4) == 0)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        if (*(u32*)data == g_local_player_guess->spawn_id)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055EE64), 0x111);
        }
        else
        {
            player = HWM_GET_PLAYER_BY_ID(*(int*)data);
            if (player != NULL)
            {
                sprintf(text, EQ_ARRAY(char, 0x0055EEA4), player->name);
                HWM_DSP_CHAT(text, 0x111);
            }
            else
            {
                HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055EE84), 0x111);
            }
        }
        EQ_METHOD(0, void, 0x004571EB, (void*), HWM_EQMAIN)();
        break;
    case 0x20E6:
        player = HWM_GET_PLAYER_BY_ID(*(int*)(data + 4));
        if (player == NULL)
            break;
        if (HWM_EQMAIN == NULL || (s8)HWM_EQMAIN[0xFE0] >= 5 ||
            HWM_FIELD(int, g_local_player_guess->actor_info, 0x68) != 0 || EQ_GLOBAL(int, 0x006B6590) != 0 ||
            HWM_FIELD(int, HWM_EQMAIN, 0xE5C) != 0 || HWM_FIELD(int, HWM_EQMAIN, 0x94) != 0)
        {
            strcpy((char*)busyTell, player->name);
            sprintf((char*)busyTell + 0x18, EQ_ARRAY(char, 0x0055EEE4), g_local_player_guess->name);
            HWM_SEND(0x2016, busyTell, 0x118, 1);
            duelMessage[0] = player->spawn_id;
            duelMessage[1] = g_local_player_guess->spawn_id;
            HWM_SEND(0x20DB, duelMessage, 8, 1);
            goto trade_interest;
        }
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        if (g_pc_guess->inventory_items[0] != NULL &&
            (player->type == 1 || EQ_CDECL(int, 0x004A0A4C, (EQ_Item*, int))(g_pc_guess->inventory_items[0], 0) != 0))
        {
            EQ_METHOD(4, void, 0x00463651, (void*, int, int, int, int), HWM_EQMAIN)(0, 3000, 1, 1);
            g_displayManager_guess[0x3C] = 0;
        }
        if (g_game_state_guess == 2)
        {
            g_game_state_guess = 1;
            if (HWM_EQMAIN != NULL)
                HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        }
        ((HWMWindow_guess*)HWM_EQMAIN)->Deactivate_guess();
        EQ_GLOBAL(EQPlayer*, 0x006E4D88) = player;
        HWM_EQMAIN[0xFE1] = HWM_EQMAIN[0xFE0];
        HWM_EQMAIN[0xFE0] = 4;
        HWM_FIELD(int, HWM_EQMAIN, 0x685E4) = 1;
        HWM_FIELD(int, HWM_EQMAIN, 0x6C908) = 1;
        ((HWMWindow_guess*)HWM_EQMAIN)->Activate_guess();
        EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = player;
        HWM_FIELD(int, HWM_EQMAIN, 0x685EC) = 0;
        HWM_FIELD(int, HWM_EQMAIN, 0x685F0) = 0;
        HWM_FIELD(int, HWM_EQMAIN, 0xE5C)++;
        if (HWM_FIELD(int, HWM_EQMAIN, 0xE5C) != 1)
        {
            errorMessage.unknown0x00 = -4;
            errorMessage.unknown0x04 = HWM_FIELD(int, HWM_EQMAIN, 0xE5C);
            errorMessage.reason = 0x11;
            HWM_SEND(0x2147, &errorMessage, sizeof(MsgClientError), 1);
        }
        HWM_FIELD(int, HWM_EQMAIN, 0xE5C) = 1;
        if (g_displayManager_guess[0x3D] == 0)
            break;
        coinMove.from_slot = 0;
        coinMove.to_slot = 3;
        if (g_pc_guess->platinum_cursor > 0)
        {
            coinMove.from_coin_type = 3;
            coinMove.to_coin_type = 3;
            coinMove.amount = g_pc_guess->platinum_cursor;
            HWM_SEND(0x212D, &coinMove, sizeof(MsgMoveCoin), 1);
            HWM_FIELD(int, HWM_EQMAIN, 0x6C52C) += g_pc_guess->platinum_cursor;
            g_pc_guess->platinum_cursor = 0;
        }
        if (g_pc_guess->gold_cursor > 0)
        {
            coinMove.from_coin_type = 2;
            coinMove.to_coin_type = 2;
            coinMove.amount = g_pc_guess->gold_cursor;
            HWM_SEND(0x212D, &coinMove, sizeof(MsgMoveCoin), 1);
            HWM_FIELD(int, HWM_EQMAIN, 0x6C530) += g_pc_guess->gold_cursor;
            g_pc_guess->gold_cursor = 0;
        }
        if (g_pc_guess->silver_cursor > 0)
        {
            coinMove.from_coin_type = 1;
            coinMove.to_coin_type = 1;
            coinMove.amount = g_pc_guess->silver_cursor;
            HWM_SEND(0x212D, &coinMove, sizeof(MsgMoveCoin), 1);
            HWM_FIELD(int, HWM_EQMAIN, 0x6C534) += g_pc_guess->silver_cursor;
            g_pc_guess->silver_cursor = 0;
        }
        if (g_pc_guess->copper_cursor > 0)
        {
            coinMove.from_coin_type = 0;
            coinMove.to_coin_type = 0;
            coinMove.amount = g_pc_guess->copper_cursor;
            HWM_SEND(0x212D, &coinMove, sizeof(MsgMoveCoin), 1);
            HWM_FIELD(int, HWM_EQMAIN, 0x6C538) += g_pc_guess->copper_cursor;
            g_pc_guess->copper_cursor = 0;
        }
        g_displayManager_guess[0x3D] = 0;
        break;
    case 0x2125:
        EQ_CDECL(void, 0x004942E4, (void*, u32, u8*, u32))(connection, 0x2125, data, size);
        break;
    case 0x2116:
        if (HWM_EQMAIN == NULL)
            break;
        sprintf(text, EQ_ARRAY(char, 0x0055EE4C), data);
        HWM_DSP_CHAT(text, 0xF);
        break;
    case 0x20F5:
        EQ_CDECL(void, 0x004939EB, (void*, u32, u8*, u32))(connection, 0x20F5, data, size);
        break;
    case 0x20F3:
        if (HWM_FIELD(int, this, 4) != 5)
            break;
        player = HWM_GET_PLAYER_BY_ID(*(u16*)data);
        if (player == NULL)
        {
            HWM_SEND(0x2141, data, 2, 1);
            break;
        }
        EQ_CDECL(void, 0x004E0FC7, (EQPlayer*))(player);
        EQ_CDECL(void, 0x004A3717, (float*, u8*, EQPlayer*))(&player->y, data + 2, player);
        EQ_CDECL(void, 0x004E1109, (EQPlayer*))(player);
        if ((u32)(HWM_FIELD(int, g_display_guess, 0xBC) - HWM_FIELD(int, player->actor_info, 0x19C)) > 12000)
            EQ_METHOD(4, void, 0x004AE653, (void*, EQPlayer*, float, float, float), g_display_guess)(
                player, player->y, player->x, player->z);
        HWM_FIELD(int, player->actor_info, 0x19C) = HWM_FIELD(int, g_display_guess, 0xBC);
        HWM_UPDATE_ACTOR(player);
        break;
    case 0x213D:
        if (HWM_EQMAIN == NULL)
            break;
        if (g_pc_guess == NULL)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        switch (data[4])
        {
        case 0:
            g_pc_guess->copper += *(int*)(data + 8);
            break;
        case 1:
            g_pc_guess->silver += *(int*)(data + 8);
            break;
        case 2:
            g_pc_guess->gold += *(int*)(data + 8);
            break;
        case 3:
            g_pc_guess->platinum += *(int*)(data + 8);
            break;
        }
        goto zone_eqmain_dirty;
    case 0x214A:
        EQ_CDECL(void, 0x0049341C, (void*, u32, u8*, u32))(connection, 0x214A, data, size);
        break;
    case 0x2145:
        if (HWM_EQMAIN == NULL)
            break;
        EQ_METHOD(0, void, 0x00464B15, (void*), HWM_EQMAIN)();
        break;
    case 0x213E:
        if (HWM_EQMAIN == NULL || g_pc_guess == NULL)
            goto trade_slot_done;
        HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
        if (*(short*)(data + 4) < 0)
            break;
        window = HWM_WINDOW(0x006018C0);
        if (window != NULL && window->isOpen_guess != 0)
            window->Deactivate_guess();
        window = HWM_WINDOW(0x00604984);
        if (window != NULL && window->isOpen_guess != 0)
        {
            window->forcedClose_guess = 1;
            HWM_WINDOW(0x00604984)->Deactivate_guess();
        }
        if (*(short*)(data + 4) < 30)
        {
            switch (data[6])
            {
            case 0:
                newItem = (EQ_Item*)new EquipmentAllocation_guess;
                memcpy(newItem, data + 7, 0x124);
                if (newItem->item_nr == 0 || ((u8*)newItem)[0xC2] == 0xD)
                {
                    HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
                    operator delete(newItem);
                    break;
                }
                if (EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
                {
                    sprintf(text, EQ_ARRAY(char, 0x0055E2DC), newItem->item_nr);
                    HWM_DSP_CHAT(text, 0xD);
                    newItem->icon_nr = 500;
                }
                g_pc_guess->inventory_items[*(short*)(data + 4)] = newItem;
                break;
            case 1:
                newItem = (EQ_Item*)new ContainerAllocation_guess;
                memcpy(newItem, data + 7, 0xDC);
                if (newItem->item_nr == 0)
                {
                    HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
                    if (newItem == NULL)
                        break;
                    EQ_METHOD(1, void*, 0x004A0069, (void*, u8), newItem)(1);
                    break;
                }
                ((u8*)newItem)[0xD6] = 0;
                for (index = 0; index < 10; index++)
                    HWM_FIELD(EQ_Item*, newItem, 0xAC + index * 4) = NULL;
                if (EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
                {
                    sprintf(text, EQ_ARRAY(char, 0x0055E2F8), newItem->item_nr);
                    HWM_DSP_CHAT(text, 0xD);
                    newItem->icon_nr = 500;
                }
                g_pc_guess->inventory_items[*(short*)(data + 4)] = newItem;
                break;
            case 2:
                newItem = (EQ_Item*)new NoteAllocation_guess;
                memcpy(newItem, data + 7, 0xD0);
                if (newItem->item_nr == 0)
                {
                    HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
                    if (newItem == NULL)
                        break;
                    EQ_METHOD(1, void*, 0x004A0085, (void*, u8), newItem)(1);
                    break;
                }
                if (EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
                {
                    sprintf(text, EQ_ARRAY(char, 0x0055E6E8), newItem->item_nr);
                    HWM_DSP_CHAT(text, 0xD);
                    newItem->icon_nr = 500;
                }
                g_pc_guess->inventory_items[*(short*)(data + 4)] = newItem;
                break;
            }
            EQ_CDECL(void, 0x004A204D, (CharacterProfile*))(g_pc_guess);
            goto trade_slot_done;
        }
        *(short*)(data + 4) -= 30;
        object = (u8*)g_pc_guess->inventory_items[*(short*)(data + 4) / 10];
        if (object == NULL)
            goto trade_slot_done;
        if (object[0x81] != 1)
            goto trade_slot_done;
        if (HWM_FIELD(u8*, object, 0xAC + *(short*)(data + 4) % 10 * 4) != NULL)
        {
            operator delete(HWM_FIELD(u8*, object, 0xAC + *(short*)(data + 4) % 10 * 4));
            HWM_FIELD(u8*, object, 0xAC + *(short*)(data + 4) % 10 * 4) = NULL;
        }
        switch (data[6])
        {
        case 0:
            newItem = (EQ_Item*)new EquipmentAllocation_guess;
            memcpy(newItem, data + 7, 0x124);
            if (newItem->item_nr == 0)
            {
                HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
                operator delete(newItem);
                break;
            }
            HWM_FIELD(EQ_Item*, object, 0xAC + *(short*)(data + 4) % 10 * 4) = newItem;
            break;
        case 2:
            newItem = (EQ_Item*)new NoteAllocation_guess;
            memcpy(newItem, data + 7, 0xD0);
            if (newItem->item_nr == 0)
            {
                HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
                if (newItem == NULL)
                    break;
                EQ_METHOD(1, void*, 0x004A0085, (void*, u8), newItem)(1);
                break;
            }
            HWM_FIELD(EQ_Item*, object, 0xAC + *(short*)(data + 4) % 10 * 4) = newItem;
            break;
        }
    trade_slot_done:
        EQ_CDECL(void, 0x004A2816, (CharacterProfile*))(g_pc_guess);
        break;
    case 0x214B:
        if (*(short*)data == 0x11)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055EE04), 0x10E);
            break;
        }
        if (*(short*)data != 0x23)
            break;
        HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055EE28), 0x10E);
        break;
    case 0x217A:
        newItem = (EQ_Item*)new NoteAllocation_guess;
        memcpy(newItem, data, 0xD0);
        if (HWM_FIELD(int, this, 4) == 5 && EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E6E8), newItem->item_nr);
            HWM_DSP_CHAT(text, 0xD);
            newItem->icon_nr = 500;
        }
        if (EQ_CDECL(bool, 0x00494884, (EQ_Item*, int))(newItem, 0))
            break;
        if (newItem == NULL)
            break;
        EQ_METHOD(1, void*, 0x004A0085, (void*, u8), newItem)(1);
        break;
    case 0x2179:
        newItem = (EQ_Item*)new ContainerAllocation_guess;
        memcpy(newItem, data, 0xDC);
        if (HWM_FIELD(int, this, 4) == 5 && EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E2F8), newItem->item_nr);
            HWM_DSP_CHAT(text, 0xD);
            newItem->icon_nr = 500;
        }
        if (EQ_CDECL(bool, 0x00494884, (EQ_Item*, int))(newItem, 0))
            break;
        if (newItem == NULL)
            break;
        EQ_METHOD(1, void*, 0x004A0069, (void*, u8), newItem)(1);
        break;
    case 0x2178:
        newItem = (EQ_Item*)new EquipmentAllocation_guess;
        memcpy(newItem, data, 0x124);
        if (((u8*)newItem)[0xC2] == 0xD || newItem->item_nr < 1000)
        {
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055E314), 0xD);
            operator delete(newItem);
            errorMessage.unknown0x00 = -2;
            errorMessage.unknown0x04 = -2;
            errorMessage.reason = 0x12;
            HWM_SEND(0x2147, &errorMessage, sizeof(MsgClientError), 1);
            break;
        }
        if (HWM_FIELD(int, this, 4) == 5 && EQ_ARRAY(int, 0x006B32B4)[newItem->icon_nr] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E2DC), newItem->item_nr);
            HWM_DSP_CHAT(text, 0xD);
            newItem->icon_nr = 500;
        }
        if (((u8*)newItem)[0xD8] != 0)
        {
            EQ_GLOBAL(EQ_Item*, 0x005DFBBC) = newItem;
            if (EQ_CDECL(int, 0x004A1D83, (EQ_Item*))(newItem) != 0 && ((u8*)newItem)[0xDA] == 0)
            {
                sprintf(text, EQ_ARRAY(char, 0x0055E704), newItem->item_nr);
                HWM_DSP_CHAT(text, 0xD);
                operator delete(newItem);
                errorMessage.unknown0x00 = -2;
                errorMessage.unknown0x04 = -2;
                errorMessage.reason = 0x13;
                HWM_SEND(0x2147, &errorMessage, sizeof(MsgClientError), 1);
                break;
            }
        }
        if (EQ_CDECL(bool, 0x00494884, (EQ_Item*, int))(newItem, 0))
            break;
        operator delete(newItem);
        break;
    case 0x214D:
        sprintf(fileName, EQ_ARRAY(char, 0x0055EDFC), data);
        if (EQ_CDECL(int, 0x004A3D04, (char*))(fileName) != 1)
            break;
        EQ_METHOD(7, void, 0x00414CED, (void*, EQPlayer*, u8*, float, float, float, float, const char*), g_display_guess)(
            g_local_player_guess, data, *(float*)(data + 0x20), *(float*)(data + 0x24),
            /* x, y, z, heading */ *(float*)(data + 0x28), *(float*)(data + 0x2C),
            EQ_ARRAY(char, 0x0055EDEC));
        break;
    case 0x2180:
        parseDone = 0;
        parsePos = 0;
        EQ_CDECL(int, 0x004911D3, (u8*))(data + 4);
        do
        {
            if (EQ_CDECL(int, 0x00491137, (u8*, char*, u32*, int))(data + 4, specialLine, &parsePos, 0x200) != 0)
            {
                HWM_DSP_CHAT(specialLine, *(u16*)data);
            }
            else
            {
                if (strlen(specialLine) != 0)
                    HWM_DSP_CHAT(specialLine, *(u16*)data);
                parseDone = 1;
            }
        } while (parseDone == 0);
        break;
    case 0x21C1:
        if (*(u32*)data == g_local_player_guess->spawn_id)
            break;
        if (*(int*)(data + 4) < 0)
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055EDBC), 0xF);
        else
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055ED78), 0xF);
        break;
    case 0x21C0:
        if (*(u32*)data == g_local_player_guess->spawn_id)
            break;
        if (*(int*)(data + 4) < 0)
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055ED40), 0xF);
        else
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055ECFC), 0xF);
        break;
    case 0x21BF:
        EQ_CDECL(void, 0x00495808, (u8*))(data);
        break;
    case 0x21BE:
        if (*(u32*)data == g_local_player_guess->spawn_id)
        {
            if (*(int*)(data + 8) < 0)
                break;
            if (*(int*)(data + 4) < 0)
                break;
            if (*(int*)(data + 4) >= 0x4A)
                break;
            g_pc_guess->skills[*(int*)(data + 4)] = data[8];
            break;
        }
        if (*(int*)(data + 8) < 0)
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055ECC8), 0xF);
        else
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055EC7C), 0xF);
        break;
    case 0x21C2:
        if (*(u32*)data == g_local_player_guess->spawn_id)
        {
            if (*(float*)(data + 4) < EQ_GLOBAL(float, 0x005344A4))
                break;
            if (*(float*)(data + 4) >= EQ_GLOBAL(float, 0x00535690))
                break;
            EQ_CDECL(void, 0x004A22B6, (CharacterProfile*, float))(g_pc_guess, *(float*)(data + 4));
            sprintf(text, EQ_ARRAY(char, 0x0055EC64), g_pc_guess->level);
            HWM_DSP_CHAT(text, 0xF);
        zone_eqmain_dirty:
            HWM_FIELD(int, HWM_EQMAIN, 0xEB8) = 1;
            break;
        }
        if (*(float*)(data + 4) < EQ_GLOBAL(float, 0x0053510C))
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055EC34), 0xF);
        else
            HWM_DSP_CHAT(EQ_ARRAY(char, 0x0055EBF4), 0xF);
        break;
    case 0x2204:
        HWM_DSP_CHAT((const char*)data + 0x24, 0xF);
        break;
    case 0x21E0:
        sprintf(text, EQ_ARRAY(char, 0x0055EBE4), data);
        HWM_DSP_CHAT(text, 0xF);
        for (slot = 0; slot < 74; slot++)
        {
            if (*(short*)(data + 0x24 + slot * 2) < 0 || *(short*)(data + 0x24 + slot * 2) >= 0xFE)
                continue;
            sprintf(text, EQ_ARRAY(char, 0x0055EBD4), g_skill_records_guess[slot], *(short*)(data + 0x24 + slot * 2),
                EQ_CDECL(int, 0x004A0CA1, (short, short, int, int))(
                    *(short*)(data + 0x20), *(short*)(data + 0x22), slot, 0),
                g_skill_records_guess[slot][0x68 + *(short*)(data + 0x22)]);
            HWM_DSP_CHAT(text, 0xF);
        }
        break;
    case 0x21DD:
        if (size != 0)
            data[size - 1] = 0;
        else
            data[0] = 0;
        if (EQ_ARRAY(char, 0x006B7128)[0] == '~')
            strcpy(EQ_ARRAY(char, 0x006B7128), (const char*)data);
        else
            EQ_ARRAY(char, 0x006B7128)[0] = 0;
        strcpy(EQ_ARRAY(char, 0x006B7328), EQ_ARRAY(char, 0x006B7128));
        break;
    }
    return true;
allocation_failed:
    HWM_FIELD(int, this, 0) = 1;
    EQ_GLOBAL(HWMCriticalSectionFunction_guess, 0x00534190)((LPCRITICAL_SECTION)0x006048E8);
    return false;
}
#undef CEverQuest

// Container-derived control (TAKP SliderControl, low); only the scalar deleting destructor is rewritten here.
class SliderControl_guess
{
public:
    SliderControl_guess* ScalarDeletingDtor_guess(u8 flags);
};

// FUNCTION: TRILOGY 0x004A0069
// Cleans up the items stored in this container.
// __thiscall (TAKP name SliderControl::~SliderControl, low). Scalar deleting destructor: runs the
// EQ_Container destructor, frees the object when bit 0 of flags is set, returns this.
SliderControl_guess* SliderControl_guess::ScalarDeletingDtor_guess(u8 flags)
{
    EQ_METHOD(0, void, 0x0043405A, (void*), this)();
    if (flags & 1)
        operator delete(this);
    return this;
}

// Container-derived control (TAKP LabelControl, low); only the scalar deleting destructor is rewritten here.
class LabelControl_guess
{
public:
    LabelControl_guess* ScalarDeletingDtor_guess(u8 flags);
};

// FUNCTION: TRILOGY 0x004A0085
// Resets the interface window state owned by this control.
// __thiscall (TAKP name LabelControl::~LabelControl, low). Scalar deleting destructor: runs the base
// destructor at 0x0040101D, frees the object when bit 0 of flags is set, returns this.
LabelControl_guess* LabelControl_guess::ScalarDeletingDtor_guess(u8 flags)
{
    EQ_METHOD(0, void, 0x0040101D, (void*), this)();
    if (flags & 1)
        operator delete(this);
    return this;
}

class STMLboxControl_guess
{
public:
    STMLboxControl_guess* ScalarDeletingDtor_guess(u8 flags);
};

// FUNCTION: TRILOGY 0x004A00A1
// Removes a spawn from the zone and clears its client-side state.
STMLboxControl_guess* STMLboxControl_guess::ScalarDeletingDtor_guess(u8 flags)
{
    EQ_METHOD(0, void, 0x004B0B9F, (void*), this)();
    if (flags & 1)
        operator delete(this);
    return this;
}

// Container-derived control (TAKP ComboboxControl, low); only the scalar deleting destructor is rewritten here.
class ComboboxControl_guess
{
public:
    ComboboxControl_guess* ScalarDeletingDtor_guess(u8 flags);
};

// FUNCTION: TRILOGY 0x004A00BD
// Clears the money list associated with this interface control.
// __thiscall (TAKP name ComboboxControl::~ComboboxControl, low). Scalar deleting destructor: runs the
// EQMoneyList destructor at 0x004AD9B0, frees the object when bit 0 of flags is set, returns this.
ComboboxControl_guess* ComboboxControl_guess::ScalarDeletingDtor_guess(u8 flags)
{
    EQ_METHOD(0, void, 0x004AD9B0, (void*), this)();
    if (flags & 1)
        operator delete(this);
    return this;
}

// Container-derived control (TAKP PageControl, low); only the scalar deleting destructor is rewritten here.
class PageControl_guess
{
public:
    PageControl_guess* ScalarDeletingDtor_guess(u8 flags);
};

// FUNCTION: TRILOGY 0x004A00D9
// Cleans up the item entry managed by this list control.
// __thiscall (TAKP name PageControl::~PageControl, low). Scalar deleting destructor: runs the
// EQItemList destructor at 0x004A0396, frees the object when bit 0 of flags is set, returns this.
PageControl_guess* PageControl_guess::ScalarDeletingDtor_guess(u8 flags)
{
    EQ_METHOD(0, void, 0x004A0396, (void*), this)();
    if (flags & 1)
        operator delete(this);
    return this;
}

// FUNCTION: TRILOGY 0x004A3050
// Applies group roster changes and zone loading updates to client state.
// __cdecl, name low (takp list only; Post-Luclin UI name, Trilogy has the old UI). Returns 0 without pc, else
// switches on the int at +0xE0 of operation: 0 -> 0x004A2F02(pc, operation+0x20); 1 -> 0x004A2F7C(pc, operation+0x20);
// 3 -> 0x004A2FFC(pc, operation+0x20, operation+0x40), returned as is; 4 -> 0x004A2FC7(pc, operation); 5 -> 1; else 0.
int DispatchGroupUpdateOperation_guess(int pc, char* operation)
{
    int operationType;
    int result;

    if (pc != 0)
    {
        operationType = *(int*)(operation + 0xe0);
        if (operationType == 0)
        {
            result = EQ_CDECL(int, 0x004A2F02, (int, char*))(pc, operation + 0x20);
            return result;
        }
        if (operationType == 1)
        {
            result = EQ_CDECL(int, 0x004A2F7C, (int, char*))(pc, operation + 0x20);
            return result;
        }
        if (operationType == 3)
        {
            result = EQ_CDECL(int, 0x004A2FFC, (int, char*, char*))(pc, operation + 0x20, operation + 0x40);
            return result;
        }
        if (operationType == 4)
        {
            result = EQ_CDECL(int, 0x004A2FC7, (int, char*))(pc, operation);
            return result;
        }
        if (operationType == 5)
        {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004BEA23
// __fastcall (unused this and edx); maps a zone short name to its display name.
// Copies the fallback name into longName first, then compares shortName against about 160 table entries one by one
// with a case-insensitive compare; the first match sets matchName and jumps to copyName.
// With no match the fallback name stays in longName.
void __fastcall GetFullZoneName_guess(void* unusedThis, int unusedEdx, char* shortName, char* longName)
{
    int cmp;
    char* matchName;

    strcpy(longName, (const char*)0x0056990c);
    cmp = _strcmpi(shortName, (const char*)0x55c3e8);
    if (cmp == 0)
    {
        matchName = (char*)0x5698fc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c358);
    if (cmp == 0)
    {
        matchName = (char*)0x5698ec;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561e00);
    if (cmp == 0)
    {
        matchName = (char*)0x5698dc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53e760);
    if (cmp == 0)
    {
        matchName = (char*)0x5698cc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561dd0);
    if (cmp == 0)
    {
        matchName = (char*)0x5698bc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561df4);
    if (cmp == 0)
    {
        matchName = (char*)0x5698b0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d44);
    if (cmp == 0)
    {
        matchName = (char*)0x5698a0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d38);
    if (cmp == 0)
    {
        matchName = (char*)0x569890;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c37c);
    if (cmp == 0)
    {
        matchName = (char*)0x569888;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d68);
    if (cmp == 0)
    {
        matchName = (char*)0x569878;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561dd0);
    if (cmp == 0)
    {
        matchName = (char*)0x569868;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561dc8);
    if (cmp == 0)
    {
        matchName = (char*)0x56985c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561db0);
    if (cmp == 0)
    {
        matchName = (char*)0x569850;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5417e8);
    if (cmp == 0)
    {
        matchName = (char*)0x569840;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561dbc);
    if (cmp == 0)
    {
        matchName = (char*)0x569830;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d94);
    if (cmp == 0)
    {
        matchName = (char*)0x569820;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c5c);
    if (cmp == 0)
    {
        matchName = (char*)0x569804;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x569814);
    if (cmp == 0)
    {
        matchName = (char*)0x569804;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c39c);
    if (cmp == 0)
    {
        matchName = (char*)0x5697fc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d98);
    if (cmp == 0)
    {
        matchName = (char*)0x5697f0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561cd8);
    if (cmp == 0)
    {
        matchName = (char*)0x56954c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5697e8);
    if (cmp == 0)
    {
        matchName = (char*)0x5697d8;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53e470);
    if (cmp == 0)
    {
        matchName = (char*)0x5697cc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561cf8);
    if (cmp == 0)
    {
        matchName = (char*)0x5404fc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c90);
    if (cmp == 0)
    {
        matchName = (char*)0x5697c0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c84);
    if (cmp == 0)
    {
        matchName = (char*)0x5697b4;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c360);
    if (cmp == 0)
    {
        matchName = (char*)0x5697a4;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53e764);
    if (cmp == 0)
    {
        matchName = (char*)0x569794;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561ddc);
    if (cmp == 0)
    {
        matchName = (char*)0x569784;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d1c);
    if (cmp == 0)
    {
        matchName = (char*)0x56976c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c3ac);
    if (cmp == 0)
    {
        matchName = (char*)0x56975c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c3a4);
    if (cmp == 0)
    {
        matchName = (char*)0x569748;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d14);
    if (cmp == 0)
    {
        matchName = (char*)0x569738;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d50);
    if (cmp == 0)
    {
        matchName = (char*)0x56972c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c3c4);
    if (cmp == 0)
    {
        matchName = (char*)0x569724;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c78);
    if (cmp == 0)
    {
        matchName = (char*)0x569724;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c98);
    if (cmp == 0)
    {
        matchName = (char*)0x56970c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x569718);
    if (cmp == 0)
    {
        matchName = (char*)0x56970c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561cc0);
    if (cmp == 0)
    {
        matchName = (char*)0x5696fc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c50);
    if (cmp == 0)
    {
        matchName = (char*)0x5696f0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5696e8);
    if (cmp == 0)
    {
        matchName = (char*)0x5696d4;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561da4);
    if (cmp == 0)
    {
        matchName = (char*)0x5696c0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561aa4);
    if (cmp == 0)
    {
        matchName = (char*)0x5696b0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561cb4);
    if (cmp == 0)
    {
        matchName = (char*)0x56969c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53e754);
    if (cmp == 0)
    {
        matchName = (char*)0x56968c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d88);
    if (cmp == 0)
    {
        matchName = (char*)0x56967c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d0c);
    if (cmp == 0)
    {
        matchName = (char*)0x569674;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d04);
    if (cmp == 0)
    {
        matchName = (char*)0x569660;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c344);
    if (cmp == 0)
    {
        matchName = (char*)0x569650;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d2c);
    if (cmp == 0)
    {
        matchName = (char*)0x569644;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d80);
    if (cmp == 0)
    {
        matchName = (char*)0x569630;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d74);
    if (cmp == 0)
    {
        matchName = (char*)0x56961c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d5c);
    if (cmp == 0)
    {
        matchName = (char*)0x569608;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c300);
    if (cmp == 0)
    {
        matchName = (char*)0x5695f8;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c32c);
    if (cmp == 0)
    {
        matchName = (char*)0x5695e8;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53df4c);
    if (cmp == 0)
    {
        matchName = (char*)0x5695dc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x542294);
    if (cmp == 0)
    {
        matchName = (char*)0x5695d0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53df64);
    if (cmp == 0)
    {
        matchName = (char*)0x5695c0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c340);
    if (cmp == 0)
    {
        matchName = (char*)0x5695b0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c314);
    if (cmp == 0)
    {
        matchName = (char*)0x5695a0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c308);
    if (cmp == 0)
    {
        matchName = (char*)0x569590;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c3bc);
    if (cmp == 0)
    {
        matchName = (char*)0x569588;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561cec);
    if (cmp == 0)
    {
        matchName = (char*)0x569574;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561ce0);
    if (cmp == 0)
    {
        matchName = (char*)0x569564;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c3b4);
    if (cmp == 0)
    {
        matchName = (char*)0x56955c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561cd8);
    if (cmp == 0)
    {
        matchName = (char*)0x56954c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c3d0);
    if (cmp == 0)
    {
        matchName = (char*)0x56953c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53e468);
    if (cmp == 0)
    {
        matchName = (char*)0x569534;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c320);
    if (cmp == 0)
    {
        matchName = (char*)0x569520;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561ccc);
    if (cmp == 0)
    {
        matchName = (char*)0x569510;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c390);
    if (cmp == 0)
    {
        matchName = (char*)0x569504;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561ca8);
    if (cmp == 0)
    {
        matchName = (char*)0x569504;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561ca0);
    if (cmp == 0)
    {
        matchName = (char*)0x5694f0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55f9e8);
    if (cmp == 0)
    {
        matchName = (char*)0x5694e0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53e210);
    if (cmp == 0)
    {
        matchName = (char*)0x5694d0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53df5c);
    if (cmp == 0)
    {
        matchName = (char*)0x5694b8;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c74);
    if (cmp == 0)
    {
        matchName = (char*)0x5694a8;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c68);
    if (cmp == 0)
    {
        matchName = (char*)0x569494;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c388);
    if (cmp == 0)
    {
        matchName = (char*)0x56948c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c44);
    if (cmp == 0)
    {
        matchName = (char*)0x569478;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c3c);
    if (cmp == 0)
    {
        matchName = (char*)0x56946c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c34);
    if (cmp == 0)
    {
        matchName = (char*)0x569458;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5417f4);
    if (cmp == 0)
    {
        matchName = (char*)0x569444;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c24);
    if (cmp == 0)
    {
        matchName = (char*)0x569434;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c18);
    if (cmp == 0)
    {
        matchName = (char*)0x56941c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c10);
    if (cmp == 0)
    {
        matchName = (char*)0x56940c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c374);
    if (cmp == 0)
    {
        matchName = (char*)0x5693fc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561bf8);
    if (cmp == 0)
    {
        matchName = (char*)0x5693e8;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5417e0);
    if (cmp == 0)
    {
        matchName = (char*)0x5693dc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561be8);
    if (cmp == 0)
    {
        matchName = (char*)0x5693c8;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561bdc);
    if (cmp == 0)
    {
        matchName = (char*)0x5693bc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561bd0);
    if (cmp == 0)
    {
        matchName = (char*)0x5693ac;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561bc8);
    if (cmp == 0)
    {
        matchName = (char*)0x5693a4;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561c08);
    if (cmp == 0)
    {
        matchName = (char*)0x569398;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561d24);
    if (cmp == 0)
    {
        matchName = (char*)0x56938c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561bbc);
    if (cmp == 0)
    {
        matchName = (char*)0x56937c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561bb4);
    if (cmp == 0)
    {
        matchName = (char*)0x569368;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x541800);
    if (cmp == 0)
    {
        matchName = (char*)0x569354;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5417d4);
    if (cmp == 0)
    {
        matchName = (char*)0x569344;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561ba4);
    if (cmp == 0)
    {
        matchName = (char*)0x569330;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b98);
    if (cmp == 0)
    {
        matchName = (char*)0x56931c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53df50);
    if (cmp == 0)
    {
        matchName = (char*)0x56930c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b90);
    if (cmp == 0)
    {
        matchName = (char*)0x5692fc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b88);
    if (cmp == 0)
    {
        matchName = (char*)0x5692ec;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b80);
    if (cmp == 0)
    {
        matchName = (char*)0x5692e4;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b78);
    if (cmp == 0)
    {
        matchName = (char*)0x5692dc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b6c);
    if (cmp == 0)
    {
        matchName = (char*)0x5692cc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c36c);
    if (cmp == 0)
    {
        matchName = (char*)0x5692bc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b64);
    if (cmp == 0)
    {
        matchName = (char*)0x5692b4;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b5c);
    if (cmp == 0)
    {
        matchName = (char*)0x5692a4;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b54);
    if (cmp == 0)
    {
        matchName = (char*)0x56929c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b4c);
    if (cmp == 0)
    {
        matchName = (char*)0x56928c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b3c);
    if (cmp == 0)
    {
        matchName = (char*)0x569274;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b30);
    if (cmp == 0)
    {
        matchName = (char*)0x56925c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c464);
    if (cmp == 0)
    {
        matchName = (char*)0x56924c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53df0c);
    if (cmp == 0)
    {
        matchName = (char*)0x569240;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b24);
    if (cmp == 0)
    {
        matchName = (char*)0x569234;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53df40);
    if (cmp == 0)
    {
        matchName = (char*)0x569224;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b18);
    if (cmp == 0)
    {
        matchName = (char*)0x569214;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b0c);
    if (cmp == 0)
    {
        matchName = (char*)0x569204;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561b00);
    if (cmp == 0)
    {
        matchName = (char*)0x5691f0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561af4);
    if (cmp == 0)
    {
        matchName = (char*)0x5691e0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561ae8);
    if (cmp == 0)
    {
        matchName = (char*)0x5691d4;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561ae0);
    if (cmp == 0)
    {
        matchName = (char*)0x5691c4;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561adc);
    if (cmp == 0)
    {
        matchName = (char*)0x5691bc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53df18);
    if (cmp == 0)
    {
        matchName = (char*)0x5691a8;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561acc);
    if (cmp == 0)
    {
        matchName = (char*)0x569194;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561ac4);
    if (cmp == 0)
    {
        matchName = (char*)0x569184;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53df30);
    if (cmp == 0)
    {
        matchName = (char*)0x569170;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53df24);
    if (cmp == 0)
    {
        matchName = (char*)0x569160;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x53df04);
    if (cmp == 0)
    {
        matchName = (char*)0x569150;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561ab8);
    if (cmp == 0)
    {
        matchName = (char*)0x569138;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561ab0);
    if (cmp == 0)
    {
        matchName = (char*)0x56912c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a84);
    if (cmp == 0)
    {
        matchName = (char*)0x569114;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x569124);
    if (cmp == 0)
    {
        matchName = (char*)0x569114;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a7c);
    if (cmp == 0)
    {
        matchName = (char*)0x569108;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a74);
    if (cmp == 0)
    {
        matchName = (char*)0x5690fc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a6c);
    if (cmp == 0)
    {
        matchName = (char*)0x5690ec;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a64);
    if (cmp == 0)
    {
        matchName = (char*)0x5690dc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a5c);
    if (cmp == 0)
    {
        matchName = (char*)0x5690cc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a54);
    if (cmp == 0)
    {
        matchName = (char*)0x5690bc;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a48);
    if (cmp == 0)
    {
        matchName = (char*)0x5690ac;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a40);
    if (cmp == 0)
    {
        matchName = (char*)0x5690a0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a30);
    if (cmp == 0)
    {
        matchName = (char*)0x569090;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a28);
    if (cmp == 0)
    {
        matchName = (char*)0x569090;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a20);
    if (cmp == 0)
    {
        matchName = (char*)0x569090;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a38);
    if (cmp == 0)
    {
        matchName = (char*)0x569080;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a14);
    if (cmp == 0)
    {
        matchName = (char*)0x569070;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a08);
    if (cmp == 0)
    {
        matchName = (char*)0x56905c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5619fc);
    if (cmp == 0)
    {
        matchName = (char*)0x569050;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5619f0);
    if (cmp == 0)
    {
        matchName = (char*)0x569040;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x55c3dc);
    if (cmp == 0)
    {
        matchName = (char*)0x569028;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5619bc);
    if (cmp == 0)
    {
        matchName = (char*)0x569014;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5619e4);
    if (cmp == 0)
    {
        matchName = (char*)0x569004;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5619dc);
    if (cmp == 0)
    {
        matchName = (char*)0x568ff8;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5619d4);
    if (cmp == 0)
    {
        matchName = (char*)0x568fe8;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5619c8);
    if (cmp == 0)
    {
        matchName = (char*)0x568fd4;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5619b4);
    if (cmp == 0)
    {
        matchName = (char*)0x568fc8;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5619a8);
    if (cmp == 0)
    {
        matchName = (char*)0x568fb0;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x5619a0);
    if (cmp == 0)
    {
        matchName = (char*)0x568f9c;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561994);
    if (cmp == 0)
    {
        matchName = (char*)0x568f88;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x56198c);
    if (cmp == 0)
    {
        matchName = (char*)0x568f74;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561984);
    if (cmp == 0)
    {
        matchName = (char*)0x568f60;
        goto copyName;
    }
    cmp = _strcmpi(shortName, (const char*)0x561a9c);
    if (cmp != 0)
        return;

    matchName = (char*)0x568f54;

copyName:
    strcpy(longName, matchName);
    return;
}


// FUNCTION: TRILOGY 0x004caec9
// Restores the local character name when the pending-name placeholder is in use.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (by address)
// __thiscall (HandleWorldMessage passes its `this`), `this` and the argument unused. When the name buffer at
// 0x005FE968 is empty or "PENDING", copies the profile's name into it.
void CEverQuest::SetPendingName_guess(int unused)
{
    if (g_name_buffer_guess[0] == '\0' || strcmp(g_name_buffer_guess, "PENDING") == 0)
        strcpy(g_name_buffer_guess, g_pc_guess->name);
}

// FUNCTION: TRILOGY 0x004caefb
// ORIGINAL FILE: EverQuest/EQHandleMsg.cpp (from Mac debug map)
// Prints why a group invite was not accepted: considering another group, already grouped, or rejected.
// @early-stop: VC6 loads the message into ECX and the status into EAX (the original is the other way round), and one xor edx is left for the EQ_THISCALL dummy EDX
void __fastcall HandleGroupInviteResponse_guess(void* receiver, int unusedEdx, int message)
{
    void* state;
    char text[120];

    state = EQ_GLOBAL(void*, 0x006E4E44);
    if (state != NULL && *(s32*)((u8*)state + 0x10) != 0)
    {
        switch (*(u8*)(message + 0x3C))
        {
        case 1:
            sprintf(text, EQ_ARRAY(char, 0x0056CB38), message + 0x1E);
            break;
        case 2:
            sprintf(text, EQ_ARRAY(char, 0x0056CB6C), message + 0x1E);
            break;
        case 3:
            sprintf(text, EQ_ARRAY(char, 0x0056CB8C), message + 0x1E);
            break;
        default:
            return;
        }

        CEverQuest_print_chat_guess((CEverQuest*)receiver, 0, text);
    }
}

// FUNCTION: TRILOGY 0x004e87fd
// Loads the server's list of names treated as evil processes.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name fillEvilProcessNames, low)
// __cdecl. Clears the 4096-byte table at 0x006EE7E4 and copies the message's names into it: a short count,
// then 64-byte names from +2. The count is not checked against the table size (64 entries) and is re-read
// on every round.
void fillEvilProcessNames_guess(short* msg)
{
    int i;

    memset(g_evil_process_names_guess, 0, 0x1000);
    if (msg != NULL)
    {
        for (i = 0; i < *msg; i++)
            strcpy(g_evil_process_names_guess + i * 64, (char*)(msg + 1) + i * 64);
    }
}

// FUNCTION: TRILOGY 0x004f46a0
// ORIGINAL FILE: unknown (subsystem from the call graph)
// __cdecl. Stores `value` in the dword at +0x618 of `object` and returns the old value.
int ExchangeField618_guess(char* object, int value)
{
    int old = *(int*)(object + 0x618);

    *(int*)(object + 0x618) = value;
    return old;
}
