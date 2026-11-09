/*
 * EQEffect.cpp - EQEffect (Mac: Source/PC Source/EverQuest/EQEffect.cpp).
 */
#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/display.h"

#include "trilogy/layout/eq_effect.h"   /* EQEffect_guess; g_effect_head_guess = head of the effect list */

// FUNCTION: TRILOGY 0x00490c8e
// Creates or copies a visual effect with its appearance, optional light, and lifetime.
// ORIGINAL FILE: EverQuest/EQEffect.cpp (from Mac debug map)
EQEffect_guess::EQEffect_guess(void* source, char* actorDefName, short type,
                               const u32* position, u32 actorId, u32 actorData,
                               char* spriteTint, u32 owner, short x, short y,
                               float actorScale, float scale)
{
    EQEffect_guess* head;
    u32* self;
    int actor;
    int now;
    int mode;
    float value;

    self = (u32*)this;
    head = g_effect_head_guess;
    if (head == 0)
    {
        g_effect_head_guess = this;
        next = 0;
        prev = 0;
    }
    else
    {
        head->prev = this;
        head = *(EQEffect_guess* volatile*)&g_effect_head_guess;
        next = head;
        prev = 0;
        g_effect_head_guess = this;
    }

    light = 0;
    if (source == 0)
    {
        self[0x16] = actorId;
        this->type = (u16)type;
        EQ_CDECL(int, 0x005224FB, (char*, const char*, const char*))
            ((char*)this + 0x30, EQ_ARRAY(char, 0x0055DCC0), actorDefName);
        self[0] = position[0];
        self[1] = position[1];
        self[2] = position[2];
        this->pitch = 0.0f;
        this->speed_guess = 0.0f;
        *(float*)((char*)this + 0x0c) = 0.0f;
        this->scale = actorScale;
        self[0x1b] = owner;
        this->velocityY_guess = 0.0f;
        this->velocityX_guess = 0.0f;
        this->velocityZ_guess = 0.0f;
        this->headingVelocity_guess = 0.0f;
        this->pitchVelocity_guess = 0.0f;
        this->roll = 0.0f;
        this->headingOffset = (u16)x;
        this->rollOffset = (u16)y;

        switch (type)
        {
        case 2:
            if (scale < *(float*)EQ_ARRAY(float, 0x005345B8))
                scale = *(float*)EQ_ARRAY(float, 0x005345B8);
            value = *(float*)EQ_ARRAY(float, 0x005345B8);
            if (actorScale != *(float*)EQ_ARRAY(float, 0x0053510C))
                value = actorScale;
            this->scale = value;
            this->speed_guess = value;
            this->headingVelocity_guess = (float)(int)x;
            this->pitchVelocity_guess = (float)(int)y;
            break;
        }
    }
    else
    {
        EQEffect_guess* copy = (EQEffect_guess*)source;
        self[0x16] = copy->source_bits_guess;
        this->type = copy->type;
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))
            ((char*)this + 0x30, copy->actorDefName_guess);
        EQ_CDECL(void*, 0x00521B10, (void*, const void*, u32))
            (this, source, 0x30);
        this->scale = copy->scale;
        this->headingOffset = copy->headingOffset;
        this->rollOffset = copy->rollOffset;
        self[0x1b] = copy->angleRate_bits_guess;
    }

    self[0x17] = actorData;
    actor = EQ_METHOD(9, int, 0x004112DF, (CDisplay*, char*, float, float, float, float,
                         float, float, int, int), g_cdisplay_guess)((char*)this + 0x30,
         this->y, this->x,
         this->z, this->heading,
         this->pitch, 0.0f, 1, 0);
    self[0x18] = actor;
    if (actor == 0)
        return;

    EQ_METHOD(2, void, 0x004105E8, (CDisplay*, void*, void*), g_cdisplay_guess)((void*)actor, 0);
    EQ_METHOD(3, void, 0x0040C113, (CDisplay*, void*, float, int), g_cdisplay_guess)((void*)self[0x18], scale, 0);
    if (spriteTint != 0)
    {
        if (spriteTint[0] == 0 && spriteTint[1] == 0 && spriteTint[2] == 0)
        {
            spriteTint[0] = -1;
            spriteTint[1] = -1;
            spriteTint[2] = -1;
        }
        EQ_METHOD(2, void, 0x00413FFD, (CDisplay*, u8*, void*), g_cdisplay_guess)((u8*)spriteTint, (void*)self[0x18]);
    }

    now = EQ_CDECL(int, 0x004A461E, (void))();
    mode = (int)type;
    self[0x1d] = now;
    switch (mode)
    {
    case 0:
        self[0x1c] = 0;
        break;
    case 2:
        if (self[0x16] == 0)
        {
            self[0x1c] = 0;
        }
        else
        {
            now += 2000;
            self[0x1c] = now;
        }
        break;
    case 1:
        light = EQ_METHOD(5, void*, 0x00410725, (CDisplay*, int, float, float, float, float), g_cdisplay_guess)(6, *(float*)EQ_ARRAY(float, 0x00535688),
             this->y, this->x,
             this->z);
        now = *(int*)((char*)this + 0x74) + 750;
        self[0x1c] = now;
        break;
    default:
        self[0x1c] = mode + now;
        break;
    }
}

// FUNCTION: TRILOGY 0x00490ef4
// ORIGINAL FILE: EverQuest/EQEffect.cpp (from Mac debug map)
// __thiscall. Unlinks the effect from the global list, then deletes its light and actor.
EQEffect_guess::~EQEffect_guess()
{
    if (prev != 0 && next != 0)
    {
        prev->next = next;
        next->prev = prev;
    }
    else if (prev == 0 && next != 0)
    {
        next->prev = 0;
        g_effect_head_guess = next;
    }
    else if (next == 0 && prev != 0)
    {
        prev->next = 0;
    }
    else
    {
        next = 0;
        prev = 0;
        g_effect_head_guess = 0;
    }
    if (light != 0)
        EQ_METHOD(1, void, 0x00410970, (CDisplay*, void*), g_cdisplay_guess)(light);
    if (actor != 0)
        EQ_METHOD(1, void, 0x00411715, (CDisplay*, void*), g_cdisplay_guess)(actor);
}
