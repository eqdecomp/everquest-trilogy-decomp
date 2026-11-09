// EQItemList.cpp - item actor list lookups.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQItemList.cpp (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/eq_actor_lists.h"
#include "trilogy/display.h"

// FUNCTION: TRILOGY 0x004a02e5
// Initializes a world item entry with a default sword appearance and registers it in the item list.
// ORIGINAL FILE: EverQuest/EQItemList.cpp (from Mac debug map)
void* __fastcall EQItemList_ctor_guess(void* self)
{
    void* head;
    unsigned short* word;
    unsigned char* byte;
    int count;

    head = EQ_GLOBAL(void*, 0x006E4D5C);
    if (head == NULL)
    {
        EQ_GLOBAL(void*, 0x006E4D5C) = self;
        *(void**)((char*)self + 4) = NULL;
        *(void**)self = NULL;
    }
    else
    {
        *(void**)head = self;
        head = *(void* volatile*)&EQ_GLOBAL(void*, 0x006E4D5C);
        *(void**)((char*)self + 4) = head;
        *(void**)self = NULL;
        EQ_GLOBAL(void*, 0x006E4D5C) = self;
    }

    *(float*)((char*)self + 0x28) = 0.0f;
    *(float*)((char*)self + 0x2C) = 20.0f;
    *(float*)((char*)self + 0x30) = 0.0f;
    *(float*)((char*)self + 0x34) = 0.0f;
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)self + 0x38, (const char*)0x0056015C);
    *(unsigned int*)((char*)self + 0x6C) |= 0xFFFFFFFF;
    *(short*)((char*)self + 0xE8) = -1;
    *(short*)((char*)self + 0x78) = -1;
    *(unsigned short*)((char*)self + 8) = 0;
    *(unsigned int*)((char*)self + 0x0C) = 0;
    count = 10;
    *(unsigned int*)((char*)self + 0x70) = 0;
    *(unsigned int*)((char*)self + 0xE4) = 10;
    *(unsigned int*)((char*)self + 0xEC) = 0;
    *(unsigned int*)((char*)self + 0x24) = 0;
    *(unsigned char*)((char*)self + 0x74) = 0;
    *(unsigned char*)((char*)self + 0x76) = 0;
    *(unsigned short*)((char*)self + 0x7A) = 0;
    *(unsigned char*)((char*)self + 0x77) = 0;
    *(unsigned char*)((char*)self + 0x75) = 0;
    *(unsigned short*)((char*)self + 0x7C) = 0;
    byte = (unsigned char*)((char*)self + 0x7E);
    word = (unsigned short*)((char*)self + 0x56);
    do
    {
        *word |= 0xFFFF;
        *byte = 0;
        word++;
        byte += 10;
        count--;
    } while (count != 0);
    return self;
}

// FUNCTION: TRILOGY 0x004a0396
// Removes an item entry from tracking and cleans up its associated actor.
// ORIGINAL FILE: EverQuest/EQItemList.cpp (from Mac debug map)
// Unlinks the item from the doubly linked list (+0 prev, +4 next, head 0x006E4D5C) and deletes its
// actor (+0x24). Written as a __fastcall free function because eq_actor_lists.h declares no destructor.
void __fastcall EQItemList_destructor_guess(int *self)
{
    if (self[0] != 0 && self[1] != 0)
    {
        *(int *)(self[0] + 4) = self[1];
        *(int *)self[1] = self[0];
    }
    else if (self[0] == 0 && self[1] != 0)
    {
        *(int *)self[1] = 0;
        EQ_GLOBAL(int, 0x006E4D5C) = self[1];
    }
    else if (self[1] == 0 && self[0] != 0)
    {
        *(int *)(self[0] + 4) = 0;
    }
    else
    {
        self[1] = 0;
        self[0] = 0;
        EQ_GLOBAL(int, 0x006E4D5C) = 0;
    }
    if (self[9] != 0)
        EQ_METHOD(1, void, 0x00411715, (int, int), EQ_GLOBAL(int, 0x006E4DE0))(self[9]);
}

// FUNCTION: TRILOGY 0x004a03f7
// The item list is a singly linked list at +4; this lookup compares the key at +0x0c.
int EQItemList::find_item_actor_guess(int actor)
{
    int item = EQ_GLOBAL(int, 0x006E4D5C);
    while (true)
    {
        if (item == 0)
            return 0;
        if (*(int *)(item + 0x0C) == actor)
            break;
        item = *(int *)(item + 4);
    }
    return item;
}

// FUNCTION: TRILOGY 0x004a0413
// ORIGINAL FILE: Source/PC Source/EverQuest/EQItemList.cpp (from Mac debug map)
// The same list is searched using the actor pointer at +0x24.
int EQItemList::is_item_actor(int actor)
{
    int item = EQ_GLOBAL(int, 0x006E4D5C);
    while (true)
    {
        if (item == 0)
            return 0;
        if (*(int *)(item + 0x24) == actor)
            break;
        item = *(int *)(item + 4);
    }
    return item;
}

// The ground item list entry DropItem_guess builds (0xF0 bytes): the client's EQItemList constructor, only
// declared here.
struct ItemListPayload_guess
{
    char bytes[0xF0];

    // EXTERNAL: TRILOGY 0x004A02E5 ItemListPayload_guess::ItemListPayload_guess
    ItemListPayload_guess();
};

// FUNCTION: TRILOGY 0x004CB7B8
// Drops the held item and any container contents onto the ground, optionally announcing the drop.
void __fastcall DropItem_guess(void *this_ptr, int unused_edx, int show_message)
{
    if (*(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xE0) != 0)
    {
        ItemListPayload_guess *payload = new ItemListPayload_guess;
        int list = (int)payload;

        *(float *)(list + 0x28) = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y;
        *(float *)(list + 0x2C) = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x;
        *(float *)(list + 0x30) = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->z;
        short item_number = *(short *)(*(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xE0) + 0x82);
        *(char *)(list + 0x74) = 0;
        *(short *)(list + 0x08) = item_number;
        *(unsigned int *)(list + 0xE4) = (unsigned int)*(unsigned char *)(*(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xE0) + 0x7D);
        *(int *)(list + 0xEC) = 0;
        *(int *)(list + 0x70) = *(int *)(*(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xE0) + 0x8C);

        if (*(char *)(*(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xE0) + 0x81) == 0)
        {
            EQ_CDECL(void *, 0x00521B10, (void *, const void *, u32))
                ((void *)(list + 0x74), (const void *)(*(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xE0) + 0xD8), 10);
        }

        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
            ((char *)(list + 0x38), (const char *)EQ_ARRAY(char, 0x0055DCC0),
             (const char *)(*(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xE0) + 0x5F));

        {
            int index;
            for (index = 0; index < 10; index++)
            {
                *(short *)(list + 0x56 + index * 2) = -1;
                *(char *)(list + 0x7E + index * 10) = 0;
            }
        }

        if (*(char *)(*(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xE0) + 0x81) == 1)
        {
            int item = *(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xE0);
            u8 child_count = *(u8 *)(item + 0xD5);
            int index = 0;
            if (child_count > 0)
            {
                char *child_slot = (char *)item + 0xAC;
                char *properties = (char *)(list + 0x7E);
                short *item_numbers = (short *)(list + 0x56);

                do
                {
                    if (*(int *)child_slot != 0)
                    {
                        *item_numbers = *(short *)(*(int *)child_slot + 0x82);
                        *(char *)(list + 0x74) = 0;
                        if (*(char *)(*(int *)child_slot + 0x81) == 0)
                        {
                            EQ_CDECL(void *, 0x00521B10, (void *, const void *, u32))
                                ((void *)properties, (const void *)(*(int *)child_slot + 0xD8), 10);
                        }
                        EQ_CDECL(void, 0x005218BB, (void *))(*(void **)child_slot);
                        *(int *)child_slot = 0;
                    }

                    child_count = *(u8 *)(item + 0xD5);
                    index++;
                    child_slot += 4;
                    item_numbers++;
                    properties += 10;
                } while (index < (int)child_count);
            }
        }

        g_cdisplay_guess->itemOnCursor_guess = 0;
        if (show_message != 0)
        {
            char message[0x100];
            EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
                (message, (const char *)EQ_ARRAY(char, 0x0056CFE0),
                 (const char *)*(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xE0));
            EQ_METHOD(2, void, 0x004D055E, (void *, const char *, int), this_ptr)(message, 0xF);
        }

        EQ_CDECL(void, 0x005218BB, (void *))
            ((void *)*(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xE0));
        *(int *)((char *)EQ_GLOBAL(void *, 0x006E4DA8) + 0xE0) = 0;

        if (EQ_GLOBAL(char, 0x006B63F8) == 0 &&
            EQ_GLOBAL(int, 0x006B6494) == 0)
        {
            EQ_CDECL(void, 0x004E1C45, (void **, u32, void *, u32, int))
                (&EQ_GLOBAL(void *, 0x006B3194), 0x2035, (void *)list, 0xF0, 1);
        }

        if (payload != 0)
        {
            EQ_METHOD(0, void, 0x004A0396, (void *), payload)();
            EQ_CDECL(void, 0x005218BB, (void *))(payload);
        }

    }
    return;
}
