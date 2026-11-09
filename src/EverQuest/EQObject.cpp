// EQObject.cpp - object list lookup.
// ORIGINAL FILE: Source/PC Source/EQObject.cpp (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/eq_actor_lists.h"

// FUNCTION: TRILOGY 0x004b00af
// Initializes a world object from supplied data or a copy, classifying ladder and grass models.
// ORIGINAL FILE: EverQuest/EQObject.cpp (from Mac debug map)
// @early-stop: copyFrom==0 path: sprintf arg pushes hoisted above the stores and strcpy cleanup merged into sprintf's, so no add esp after the first load; actor==0 return branch polarity inverted; 3 dummy-EDX xors from EQ_THISCALL; 0.25f is an R: literal, not VA 0x535154.
EQObject* __fastcall EQObject_constructor_guess(EQObject* self, int unused,
    int copyFrom, void* sourceData, char* outputName, const char* modelName)
{
    char* object = (char*)self;
    char* source;
    char actorDefinition[40];
    float scale;
    int actor;
    EQObject* head;
    typedef int (__cdecl *T3dSetActorUserData_guess_t)(int, int);

    *object = 0;
    head = EQ_GLOBAL(EQObject*, 0x006E4D6C);
    if (head == 0)
    {
        EQ_GLOBAL(EQObject*, 0x006E4D6C) = self;
        *(int*)(object + 8) = 0;
        *(int*)(object + 4) = 0;
    }
    else
    {
        *(int*)((char*)head + 4) = (int)object;
        *(int*)(object + 8) = (int)EQ_GLOBAL(EQObject*, 0x006E4D6C);
        *(int*)(object + 4) = 0;
        EQ_GLOBAL(EQObject*, 0x006E4D6C) = self;
    }

    if (copyFrom == 0)
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))
            (object + 0x0c, outputName);
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))
            (object + 0x20, modelName);
        source = (char*)sourceData;
        *(int*)(object + 0x40) = *(int*)(source + 0x34);
        *(int*)(object + 0x44) = *(int*)(source + 0x38);
        *(float*)(object + 0x48) = *(float*)(*(int*)(source + 0x70) + 0x48) + 0.25f;
        *(int*)(object + 0x4c) = *(int*)(source + 0x40);
        *(int*)(object + 0x50) = *(int*)(source + 0x44);
        *(float*)(object + 0x3c) = 1.0f;
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
            (actorDefinition, EQ_ARRAY(char, 0x0055DCC0), object + 0x20);
        actor = EQ_METHOD(9, int, 0x004112DF, (void*, char*, float, float, float, float, float, float, int, int), EQ_GLOBAL(void*, 0x006E4DE0))(actorDefinition,
             *(float*)(object + 0x40), *(float*)(object + 0x44),
             *(float*)(object + 0x48), *(float*)(object + 0x4c),
             *(float*)(object + 0x50), 0.0f, 0, 0);
        *(int*)(object + 0x54) = actor;
        if (actor == 0)
            return self;
    }
    else
    {
        source = (char*)copyFrom;
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))
            (object + 0x0c, source + 0x0c);
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))
            (object + 0x20, source + 0x20);
        *(int*)(object + 0x40) = *(int*)(source + 0x40);
        *(int*)(object + 0x44) = *(int*)(source + 0x44);
        *(int*)(object + 0x48) = *(int*)(source + 0x48);
        *(int*)(object + 0x4c) = *(int*)(source + 0x4c);
        *(int*)(object + 0x50) = *(int*)(source + 0x50);
        scale = *(float*)(source + 0x3c);
        *(float*)(object + 0x3c) = scale;
        if (scale < EQ_GLOBAL(float, 0x005351C4))
            *(float*)(object + 0x3c) = 1.0f;
        if (*(float*)(object + 0x3c) > EQ_GLOBAL(float, 0x00535124))
            *(float*)(object + 0x3c) = 1.0f;
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
            (actorDefinition, EQ_ARRAY(char, 0x0055DCC0), object + 0x20);
        actor = EQ_METHOD(9, int, 0x004112DF, (void*, char*, float, float, float, float, float, float, int, int), EQ_GLOBAL(void*, 0x006E4DE0))(actorDefinition,
             *(float*)(object + 0x40), *(float*)(object + 0x44),
             *(float*)(object + 0x48), *(float*)(object + 0x4c),
             *(float*)(object + 0x50), 0.0f, 1, 0);
        *(int*)(object + 0x54) = actor;
    }

    *object = 7;
    if (*(int*)(object + 0x54) == 0)
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))
            (outputName, EQ_ARRAY(char, 0x0058FBA4));
    }
    else
    {
        if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, unsigned int))
            (object + 0x20, EQ_ARRAY(char, 0x0053C3E0), 6) == 0)
            *object = 6;
        if (EQ_CDECL(int, 0x005229B0, (const char*, const char*, unsigned int))
            (object + 0x0c, EQ_ARRAY(char, 0x0056825C), 6) == 0)
            *object = 8;
        EQ_GLOBAL(T3dSetActorUserData_guess_t, 0x006E538C)
            (*(int*)(object + 0x54), *(u8*)object);
        EQ_METHOD(3, void, 0x0040C113, (void*, int, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(*(int*)(object + 0x54),
             *(float*)(object + 0x3c), 1);
    }
    return self;
}

// FUNCTION: TRILOGY 0x004b02c9
// Removes an object from the active object list and requests cleanup of its associated actor.
// ORIGINAL FILE: EverQuest/EQObject.cpp (from Mac debug map)
// EQObject::~EQObject (eq_actor_lists.h declares no destructor, so a __fastcall free function with `this`
// in ECX). Unless byte +0 is 0xFF, unlinks the object from the list at 0x006E4D6C (+4 prev, +8 next),
// then deletes its actor at +0x54 through the CDisplay.
void __fastcall EQObject_destructor_guess(EQObject* self)
{
    char* obj = (char *)self;

    if (*obj != -1)
    {
        if (*(int *)(obj + 4) != 0 && *(int *)(obj + 8) != 0)
        {
            *(int *)(*(int *)(obj + 4) + 8) = *(int *)(obj + 8);
            *(int *)(*(int *)(obj + 8) + 4) = *(int *)(obj + 4);
        }
        else if (*(int *)(obj + 4) == 0 && *(int *)(obj + 8) != 0)
        {
            *(int *)(*(int *)(obj + 8) + 4) = 0;
            EQ_GLOBAL(int, 0x006E4D6C) = *(int *)(obj + 8);
        }
        else if (*(int *)(obj + 8) == 0 && *(int *)(obj + 4) != 0)
        {
            *(int *)(*(int *)(obj + 4) + 8) = 0;
        }
        else
        {
            *(int *)(obj + 8) = 0;
            *(int *)(obj + 4) = 0;
            EQ_GLOBAL(int, 0x006E4D6C) = 0;
        }
    }
    if (*(int *)(obj + 0x54) != 0)
        EQ_METHOD(1, void, 0x00411715, (void*, int), EQ_GLOBAL(void*, 0x006E4DE0))(*(int *)(obj + 0x54));
}

// FUNCTION: TRILOGY 0x004b0334
// ORIGINAL FILE: Source/PC Source/EQObject.cpp (from Mac debug map)
// Walk the object list through +8 and compare the object identifier at +0x54.
int EQObject::get_object(int object_id)
{
    int object = EQ_GLOBAL(int, 0x006E4D6C);
    while (true)
    {
        if (object == 0)
            return 0;
        if (object_id == *(int *)(object + 0x54))
            break;
        object = *(int *)(object + 8);
    }
    return object;
}
