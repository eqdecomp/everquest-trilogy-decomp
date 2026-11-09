// unit_10008D38.c - eqgfx_dx7.dll, the address run 0x10008D38..0x1000A959 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its file name are unknown).

#include <windows.h>
#include <string.h>
#include "trilogy/eqaddr.h"

#define EQ_NAKED  __declspec(naked)

#define EQGFX_T3D_FLOAT_SQRT EQ_CDECL(float, 0x1004EF68, (float))
#define EQGFX_T3D_UNSHARE_OBJECT EQ_CDECL(void, 0x10017DAD, (void *))
#define EQGFX_T3D_SHARE_OBJECT EQ_CDECL(void, 0x10017CE8, (void *))
#define EQGFX_T3D_ALLOCATE_MEMORY EQ_CDECL(int *, 0x10050C48, (int))
#define EQGFX_T3D_REALLOCATE_MEMORY EQ_CDECL(int *, 0x10050C8C, (int *, int))
#define EQGFX_T3D_GET_POINTER_FROM_DICTIONARY EQ_CDECL(void *, 0x1000E7FE, (void *, const char *))
#define EQGFX_T3D_FREE_MEMORY EQ_CDECL(void, 0x10050CBB, (void *))
#define EQGFX_T3D_COPY_STRING EQ_CDECL(void *, 0x10068070, (char *, const char *))
#define EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY EQ_CDECL(int, 0x1000DC08, (void *, void *))
#define EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY EQ_CDECL(int, 0x1000E334, (void *, const char *))
#define EQGFX__T3D_ADD_ITEM_TO_LIST EQ_CDECL(int, 0x1001564B, (void *, void *))
#define EQGFX_T3D_FREE_OBJECT_POOL_SLOT EQ_CDECL(int, 0x10015D81, (void *, void *))
#define EQGFX_MEMCPY EQ_CDECL(void *, 0x10067D30, (void *, const void *, size_t))
#define EQGFX_STRLEN EQ_CDECL(size_t, 0x10068160, (const char *))
#define t3d_float_ratio_max_guess EQ_GLOBAL(float, 0x10076294)
#define EQGFX_ZERO_F_GUESS EQ_GLOBAL(float, 0x10076284)
#define EQGFX_DAT_10086118 EQ_ARRAY(BYTE, 0x10086118)
#define EQGFX_T3D_AI_STUB_CALLBACK ((int (__cdecl *)(void *, void *, void *))0x100749DF)
#define EQGFX_DEFAULT_ACTOR_SPRITE EQ_ARRAY(T3dActorSpriteObject_guess, 0x100F401C)
#define EQGFX_DAT_1008611C EQ_GLOBAL(BYTE, 0x1008611C)
#define EQGFX_DAT_10086120 EQ_GLOBAL(BYTE, 0x10086120)
#define EQGFX_DAT_10086114 EQ_GLOBAL(BYTE, 0x10086114)
#define EQGFX_DAT_10086114_ADDRESS EQ_ARRAY(BYTE, 0x10086114)
#define EQGFX_T3D_GET_REGION_NUMBER EQ_CDECL(int, 0x100192C4, (void *, DWORD *))
#define EQGFX_T3D_DUPLICATE_VOLUME EQ_CDECL(void *, 0x1001D516, (void *, void *))
#define EQGFX_T3D_DUPLICATE_POINT_LIGHT_EX EQ_CDECL(void *, 0x1001441D, (void *, void *, const void *, int))
#define EQGFX_T3D_UPDATE_TOUCHED_REGIONS EQ_CDECL(int, 0x10001000, (void *, void *, int))
#define EQGFX_T3D_CREATE_PARTICLE_CLOUD EQ_CDECL(void *, 0x1003FFE2, (void *, DWORD, int, int, int))
#define EQGFX_T3D_BIND_PARTICLE_CLOUD_SPAWN EQ_CDECL(void, 0x1004013B, (void *, void *))
#define EQGFX_T3D_DESTROY_PARTICLE_CLOUD EQ_CDECL(void, 0x10040217, (void *, void *))
#define EQGFX_T3D_DESTROY_POINT_LIGHT EQ_CDECL(void, 0x100144DC, (void *, void *))
#define EQGFX_T3D_DESTROY_VOLUME EQ_CDECL(void, 0x1001D5A1, (void *, void *))
#define EQGFX_T3D_APPEND_ACTOR_TO_ACTIVE_LIST EQ_CDECL(void, 0x10008F98, (DWORD, T3dActiveUpdate_guess *))
#define EQGFX_t3dPrependActorToActiveList EQ_CDECL(void, 0x10009031, (DWORD, T3dActiveUpdate_guess *))
#define EQGFX_T3D_INSERT_ACTOR_AFTER_IN_ACTIVE_LIST EQ_CDECL(void, 0x100090B8, (DWORD, T3dActiveUpdate_guess *, T3dActiveUpdate_guess *))
#define EQGFX_T3D_INSERT_ACTOR_BEFORE_IN_ACTIVE_LIST EQ_CDECL(void, 0x1000917A, (DWORD, T3dActiveUpdate_guess *, T3dActiveUpdate_guess *))
#define T3D_ERROR_INVALID_ACTOR_STATE 0xffff3d3e
#define T3D_ERROR_UNSUPPORTED_ACTOR_SPRITE 0xffff3d58
#define T3D_KIND_ACTOR_DEFINITION 0x17
#define T3D_KIND_ACTOR 0x18
#define T3D_ACTOR_FLAG_HIGH_BYTE 0x20

typedef struct T3dActorRefSetSlot_guess
{
    int *field00;
    DWORD field04;
    DWORD field08;
    DWORD field0c;
    int field10;
} T3dActorRefSetSlot_guess;

typedef struct T3dActorRefSetBlock_guess
{
    DWORD field00;
    T3dActorRefSetSlot_guess slots[0x1ff];
    struct T3dActorRefSetBlock_guess *next;
    DWORD field27f4;
    DWORD field27f8;
    DWORD field27fc;
} T3dActorRefSetBlock_guess;

typedef struct T3dActorRefSetHolder_guess
{
    DWORD field00;
    T3dActorRefSetBlock_guess *block;
} T3dActorRefSetHolder_guess;

typedef struct T3dActorListNode_guess
{
    struct T3dActorListNode_guess *previous;
    struct T3dActorListNode_guess *next;
    void *actor;
} T3dActorListNode_guess;

typedef struct T3dActorWorld_guess
{
    DWORD fields000[0xc4];
    int activeCount;
    T3dActorListNode_guess *activeHead;
    T3dActorListNode_guess *activeTail;
    T3dActorListNode_guess *freeHead;
    T3dActorListNode_guess *secondaryHead;
} T3dActorWorld_guess;

#define EQGFX_T3D_ALLOCATE_ACTOR_NODE_POOL_BLOCK EQ_CDECL(int, 0x1000935F, (T3dActorWorld_guess *))

typedef struct T3dWorldActorRefSet_guess
{
    BYTE pad000[0x314];
    DWORD field314;
    DWORD field318;
    void *actorRefSetState;
    DWORD field320;
    T3dActorRefSetBlock_guess *block;
} T3dWorldActorRefSet_guess;

typedef struct T3dActorEndDistanceItem_guess
{
    DWORD field00;
    float endDistanceSquared;
    DWORD field08;
} T3dActorEndDistanceItem_guess;

typedef struct T3dActorEndDistanceEntry_guess
{
    DWORD itemCount;
    T3dActorEndDistanceItem_guess *items;
} T3dActorEndDistanceEntry_guess;

typedef struct T3dActorEndDistanceGroup_guess
{
    BYTE pad00[0x14];
    DWORD groupCount;
    T3dActorEndDistanceEntry_guess *entries;
} T3dActorEndDistanceGroup_guess;

typedef struct T3dActorEndDistanceActor_guess
{
    DWORD kind;
    BYTE pad04[0x60];
    T3dActorEndDistanceGroup_guess *distanceGroup;
} T3dActorEndDistanceActor_guess;

typedef struct T3dActorRefSet_guess
{
    BYTE pad000[0x100];
    unsigned int objectCount;
    unsigned int objectCapacity;
    int *objects;
} T3dActorRefSet_guess;

#define T3D_ACTOR_REF_SET_GROWTH 5
#define T3D_ACTOR_FLAG_IN_DICTIONARY 0x01
#define T3D_KIND_SIMPLE_SPRITE 0x06
#define T3D_KIND_BLIT_SPRITE 0x08
#define T3D_KIND_2D_SPRITE 0x0a
#define T3D_KIND_3D_SPRITE 0x0c
#define T3D_KIND_COMPOSITE_SPRITE 0x12
#define T3D_KIND_HIERARCHICAL_SPRITE 0x14
#define T3D_KIND_DM_SPRITE 0x48
#define T3D_KIND_PARTICLE_CLOUD 0x4d
#define T3D_KIND_STRING_SPRITE 0x51
#define T3D_KIND_PARTICLE_CLOUD_SOURCE 0x4c
#define T3D_KIND_VOLUME_RADIUS 0x2a
#define T3D_KIND_VOLUME_RADIUS_ALT 0x2c
#define T3D_ACTOR_FLAGS_PRESERVED_MASK 0xe8000000
#define T3D_ACTOR_FLAGS_CREATE 0x28000000

typedef struct T3dShared_guess
{
    char pad[0x10];
    int ref_count; // +0x10
} T3dShared_guess;

typedef struct T3dActorDefinition_guess
{
    DWORD kind; // +0x00
} T3dActorDefinition_guess;

typedef struct T3dSpriteFallback_guess
{
    char pad[0x18];
    DWORD polyBase; // +0x18
} T3dSpriteFallback_guess;

typedef struct T3dSpriteInfo_guess
{
    char pad[0x20];
    DWORD count; // +0x20
    DWORD *uvCountBase; // +0x24
    DWORD field28; // +0x28
    T3dSpriteFallback_guess *fallback; // +0x2c
} T3dSpriteInfo_guess;

typedef struct T3dSprite_guess
{
    char pad[0x18];
    DWORD polyBase; // +0x18
    char pad2[0x10];
    T3dSpriteInfo_guess *info; // +0x2c
} T3dSprite_guess;

typedef struct T3dActorSpriteItem_guess
{
    DWORD field00; // +0x00
    DWORD field04; // +0x04
    DWORD tableIndex; // +0x08
} T3dActorSpriteItem_guess;

typedef struct T3dActorSpriteEntry_guess
{
    DWORD count; // +0x00
    T3dActorSpriteItem_guess *items; // +0x04
} T3dActorSpriteEntry_guess;

typedef struct T3dActorSpriteGroup_guess
{
    BYTE pad00[0x10];
    int ref_count;
    DWORD count; // +0x14
    T3dActorSpriteEntry_guess *entries; // +0x18
    int sprite_count; // +0x1c
    int (__cdecl *destroy_callback)(void *, void *, void *); // +0x20
} T3dActorSpriteGroup_guess;

typedef struct T3dActorSpriteObject_guess T3dActorSpriteObject_guess;
/* T3D_tagACTORINSTANCE: the actor layout is T3D_tagACTORINSTANCE (generated: eqgfx/layout/t3dactor.h); the old local view's members map by offset. */
#include "eqgfx/layout/t3dactor.h"

typedef struct T3dActorDataEntry_guess
{
    DWORD field00;
    void *buffer;
} T3dActorDataEntry_guess;

typedef struct T3dActorDefinitionObject_guess
{
    DWORD kind; // +0x00
    BYTE pad04[4];
    void *dictionary_key;
    BYTE flags;
    BYTE pad0d[3];
    int state;
    int entry_count;
    T3dActorDataEntry_guess *entries;
    int sprite_count;
    int (__cdecl *destroy_callback)(void *, void *, void *);
    DWORD field24;
    T3D_tagACTORINSTANCE *actor;
} T3dActorDefinitionObject_guess;

struct T3dActorSpriteObject_guess
{
    DWORD kind; // +0x00
    BYTE pad04[0x0b];
    BYTE flags; // +0x0f
    BYTE pad10[0x0c];
    T3D_tagACTORINSTANCE *ownerActor; // +0x1c
};

typedef struct T3dParticleCloudSprite_guess
{
    DWORD kind; // +0x00
    BYTE pad04[0x118];
    DWORD field11c; // +0x11c
} T3dParticleCloudSprite_guess;

/* RenderVolume_guess: the volume layout is RenderVolume_guess (generated: eqgfx/layout/render_volume.h); the old local view's members map by offset. */
#include "eqgfx/layout/render_volume.h"

typedef struct T3dActiveUpdate_guess
{
    DWORD kind; // +0x00
    char pad0[0x20];
    DWORD *node44; // +0x24
    DWORD pad1; // +0x28
    DWORD *node3a; // +0x2c
    DWORD pad2; // +0x30
    DWORD *node19; // +0x34
    char pad3[0x24];
    DWORD *node18; // +0x5c
} T3dActiveUpdate_guess;

typedef struct T3dActorDistanceItem_guess
{
    DWORD field0; // +0x00
    float endDistance; // +0x04
    DWORD field8; // +0x08
} T3dActorDistanceItem_guess;

typedef struct T3dActorDistanceEntry_guess
{
    DWORD count; // +0x00
    T3dActorDistanceItem_guess *items; // +0x04
} T3dActorDistanceEntry_guess;

typedef struct T3dActorDistanceGroup_guess
{
    char pad[0x14]; // +0x00
    DWORD count; // +0x14
    T3dActorDistanceEntry_guess *entries; // +0x18
} T3dActorDistanceGroup_guess;

// FUNCTION: EQGFX 0x10008D38
// __cdecl. Bounds-checks the sprite polygon index, then copies its V axis.
DWORD __cdecl t3dGet3DSpritePolygonVAxis(T3dSprite_guess *sprite, unsigned int index, DWORD *outAxis)
{
    T3dSpriteInfo_guess *info;
    DWORD base;
    DWORD *src;

    info = sprite->info;
    if (index >= info->count)
        return 0xFFFF3D3E;
    base = sprite->polyBase;
    if (base == 0)
        base = info->fallback->polyBase;
    src = (DWORD *)(base + index * 0x50 + 0x38);
    outAxis[0] = src[0];
    outAxis[1] = src[1];
    outAxis[2] = src[2];
    return 0;
}

// FUNCTION: EQGFX 0x10008D74
// Set the V texture direction for one 3D sprite polygon.
// __cdecl. Bounds-checks the sprite polygon index, then stores `inAxis` as its V axis; returns 0xFFFF3D3E when the index is out of range or the polygon setup fails.
DWORD __cdecl t3dSet3DSpritePolygonVAxis(T3dSprite_guess *sprite, unsigned int index, DWORD *inAxis)
{
    DWORD result;
    DWORD *dst;

    result = 0;
    if (index >= sprite->info->count)
        goto fail;
    if (sprite->polyBase == 0)
    {
        if (EQ_CDECL(int, 0x1000897D, (T3dSprite_guess *))(sprite) != 0)
        {
fail:
            result = 0xFFFF3D3E;
            goto done;
        }
    }
    dst = (DWORD *)(sprite->polyBase + index * 0x50 + 0x38);
    dst[0] = inAxis[0];
    dst[1] = inAxis[1];
    dst[2] = inAxis[2];
done:
    return result;
}

// FUNCTION: EQGFX 0x10008dba
// __cdecl. Bounds-checks the sprite polygon index, then copies its UV list.
DWORD __cdecl t3dGet3DSpritePolygonUVList(T3dSprite_guess *sprite, unsigned int index, void *dest)
{
    T3dSpriteInfo_guess *info;
    DWORD base;
    void *src;
    DWORD size;

    info = sprite->info;
    if (index >= info->count)
        return 0xFFFF3D3E;
    base = sprite->polyBase;
    if (base == 0)
        base = info->fallback->polyBase;
    size = *(DWORD *)((BYTE *)info->uvCountBase + index * 0x2c + 0x4) << 3;
    src = *(void **)(base + index * 0x50 + 0x44);
    memcpy(dest, src, size);
    return 0;
}

// FUNCTION: EQGFX 0x10008e0d
// Set the texture coordinates used by one 3D sprite polygon.
// __cdecl. Bounds-checks the sprite polygon index, then copies `uvList` (count << 3 bytes) over its UV list; returns 0xFFFF3D3E when the index is out of range or the polygon setup fails.
DWORD __cdecl t3dSet3DSpritePolygonUVList(T3dSprite_guess *sprite, unsigned int index, void *uvList)
{
    DWORD result;

    result = 0;
    if (index >= sprite->info->count)
        goto fail;
    if (sprite->polyBase == 0)
    {
        if (EQ_CDECL(int, 0x1000897D, (T3dSprite_guess *))(sprite) != 0)
        {
fail:
            result = 0xFFFF3D3E;
            goto done;
        }
    }
    EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))(
        *(void **)(sprite->polyBase + index * 0x50 + 0x44), uvList,
        *(DWORD *)((BYTE *)sprite->info->uvCountBase + index * 0x2c + 0x4) << 3);
done:
    return result;
}

// FUNCTION: EQGFX 0x10008e6b
// __cdecl. Bounds-checks the sprite polygon index, then reads it.
DWORD __cdecl t3dGet3DSpritePolygonTwoSided(T3dSprite_guess *sprite, unsigned int index)
{
    T3dSpriteInfo_guess *info;
    DWORD base;

    info = sprite->info;
    if (index >= info->count)
        return 0;
    base = sprite->polyBase;
    if (base == 0)
        base = info->fallback->polyBase;
    return *(DWORD *)(base + index * 0x50 + 0x4c);
}

// FUNCTION: EQGFX 0x10008E96
// Choose whether one 3D sprite polygon is visible from both sides.
// __cdecl. Stores twoSided at polygon[index]'s +0x4c; returns 0xffff3d3e when index is out of range or the polygon setup fails.
int __cdecl t3dSet3DSpritePolygonTwoSided(T3dSprite_guess *sprite, unsigned int index, DWORD twoSided)
{
    int result;

    result = 0;
    if (index >= sprite->info->count)
        goto fail;
    if (sprite->polyBase == 0)
    {
        if (EQ_CDECL(int, 0x1000897D, (T3dSprite_guess *))(sprite) != 0)
        {
fail:
            result = 0xffff3d3e;
            goto done;
        }
    }
    *(DWORD *)(sprite->polyBase + index * 0x50 + 0x4c) = twoSided;
done:
    return result;
}

// FUNCTION: EQGFX 0x10008ed9
// Initialize the world's actor update-list storage.
int __cdecl t3dInitializeActorNodePool_guess(T3dWorldActorRefSet_guess *world)
{
    T3dActorRefSetBlock_guess *block;
    int offset;
    int remaining;
    int result;

    result = 0;
    if (world->block == 0)
    {
        block = (T3dActorRefSetBlock_guess *)EQGFX_T3D_ALLOCATE_MEMORY(0x2800);
        world->block = block;
        if (block == 0)
        {
            result = 0xffff3d41;
        }
        else
        {
            offset = 0;
            remaining = 0x1ff;
            do
            {
                remaining--;
                ((T3dActorRefSetSlot_guess *)((BYTE *)world->block + offset))->field04 =
                    (DWORD)((BYTE *)world->block + offset + sizeof(T3dActorRefSetSlot_guess));
                ((T3dActorRefSetSlot_guess *)((BYTE *)world->block + offset))->field0c = 0;
                ((T3dActorRefSetSlot_guess *)((BYTE *)world->block + offset))->field10 = 0;
                offset += sizeof(T3dActorRefSetSlot_guess);
            } while (remaining != 0);
            world->block->next = 0;
            world->block->field27f8 = 0;
            world->block->field27fc = 0;
            world->actorRefSetState = world->block;
            world->field314 = 0;
            world->field318 = 0;
            world->field320 = 0;
        }
    }
    else
    {
        result = 0xffff3da1;
    }
    return result;
}

// FUNCTION: EQGFX 0x10008f98
// Schedule an actor at the end of the world's active update order.
int __cdecl t3dAppendActorToActiveList_guess(T3dActorWorld_guess *world, T3dActiveUpdate_guess *actor)
{
    T3dActorListNode_guess *node;
    T3dActorListNode_guess *tail;
    int result;

    result = 0;
    if (world->freeHead->next == 0)
    {
        result = EQGFX_T3D_ALLOCATE_ACTOR_NODE_POOL_BLOCK(world);
        if (result != 0)
            return result;
    }
    node = world->freeHead;
    tail = world->activeTail;
    world->freeHead = node->next;
    node->next = 0;
    node->previous = tail;
    if (tail == 0)
        world->activeHead = node;
    else
        tail->next = node;
    world->activeTail = node;
    if (world->secondaryHead == 0)
        world->secondaryHead = node;
    node->actor = actor;
    switch (actor->kind)
    {
    case 0x18:
        actor->node18 = (DWORD *)node;
        break;
    case 0x19:
        actor->node19 = (DWORD *)node;
        break;
    case 0x3a:
        actor->node3a = (DWORD *)node;
        break;
    case 0x44:
        actor->node44 = (DWORD *)node;
        break;
    }
    world->activeCount++;
    return result;
}

// FUNCTION: EQGFX 0x10009031
// Schedule an actor at the start of the world's active update order.
int __cdecl t3dPrependActorToActiveList_guess(T3dActorWorld_guess *world, T3dActiveUpdate_guess *actor)
{
    T3dActorListNode_guess *node;
    T3dActorListNode_guess *tail;
    int result;

    result = 0;
    if (world->freeHead->next == 0)
    {
        result = EQGFX_T3D_ALLOCATE_ACTOR_NODE_POOL_BLOCK(world);
        if (result != 0)
            return result;
    }
    node = world->freeHead;
    tail = world->activeHead;
    world->freeHead = node->next;
    node->previous = 0;
    node->next = tail;
    if (tail == 0)
        world->activeTail = node;
    else
        tail->previous = node;
    world->activeHead = node;
    node->actor = actor;
    switch (actor->kind)
    {
    case 0x18:
        actor->node18 = (DWORD *)node;
        break;
    case 0x19:
        actor->node19 = (DWORD *)node;
        break;
    case 0x3a:
        actor->node3a = (DWORD *)node;
        break;
    case 0x44:
        actor->node44 = (DWORD *)node;
        break;
    }
    world->activeCount++;
    return result;
}

// FUNCTION: EQGFX 0x100090B8
// Schedule an actor immediately after another actor in the update order.
// __cdecl, returns the grow result (0 unless the pool was empty). Links the free head node after
// insertBefore's list node (the world for kinds without a slot) and stores it in actor's slot.
#pragma optimize("y", off)
int __cdecl t3dInsertActorAfterInActiveList_guess(T3dActorWorld_guess *world, T3dActiveUpdate_guess *insertBefore, T3dActiveUpdate_guess *actor)
{
    T3dActorListNode_guess *node;
    T3dActorListNode_guess *previous;
    T3dActorListNode_guess *successor;
    int result;

    result = 0;
    if (world->freeHead->next == 0)
    {
        result = EQGFX_T3D_ALLOCATE_ACTOR_NODE_POOL_BLOCK(world);
    }
    node = world->freeHead;
    world->freeHead = node->next;
    node->actor = actor;
    switch (insertBefore->kind)
    {
    case 0x18:
        previous = (T3dActorListNode_guess *)insertBefore->node18;
        break;
    case 0x19:
        previous = (T3dActorListNode_guess *)insertBefore->node19;
        break;
    case 0x3a:
        previous = (T3dActorListNode_guess *)insertBefore->node3a;
        break;
    case 0x44:
        previous = (T3dActorListNode_guess *)insertBefore->node44;
        break;
    default:
        previous = (T3dActorListNode_guess *)world;
        break;
    }
    switch (actor->kind)
    {
    case 0x18:
        actor->node18 = (DWORD *)node;
        break;
    case 0x19:
        actor->node19 = (DWORD *)node;
        break;
    case 0x3a:
        actor->node3a = (DWORD *)node;
        break;
    case 0x44:
        actor->node44 = (DWORD *)node;
        break;
    }
    node->previous = previous;
    successor = previous->next;
    node->next = successor;
    previous->next = node;
    if (world->activeTail == previous)
        world->activeTail = node;
    else
        node->next->previous = node;
    if (world->secondaryHead == node->next)
        world->secondaryHead = node;
    world->activeCount++;
    return result;
}
#pragma optimize("y", on)

// FUNCTION: EQGFX 0x1000917A
// Schedule an actor immediately before another actor in the update order.
// __cdecl, returns the grow result (0 unless the pool was empty). Links a free node in front
// of insertBefore's list node (the world for kinds without a slot) and stores it in actor's slot.
int __cdecl t3dInsertActorBeforeInActiveList_guess(T3dActorWorld_guess *world, T3dActiveUpdate_guess *insertBefore, T3dActiveUpdate_guess *actor)
{
    T3dActorListNode_guess *node;
    T3dActorListNode_guess *successor;
    T3dActorListNode_guess *previous;
    int result;

    result = 0;
    if (world->freeHead->next == 0)
    {
        result = EQGFX_T3D_ALLOCATE_ACTOR_NODE_POOL_BLOCK(world);
    }
    node = world->freeHead;
    world->freeHead = node->next;
    node->actor = actor;
    switch (insertBefore->kind)
    {
    case 0x18:
        successor = (T3dActorListNode_guess *)insertBefore->node18;
        break;
    case 0x19:
        successor = (T3dActorListNode_guess *)insertBefore->node19;
        break;
    case 0x3a:
        successor = (T3dActorListNode_guess *)insertBefore->node3a;
        break;
    case 0x44:
        successor = (T3dActorListNode_guess *)insertBefore->node44;
        break;
    default:
        successor = (T3dActorListNode_guess *)world;
        break;
    }
    switch (actor->kind)
    {
    case 0x18:
        actor->node18 = (DWORD *)node;
        break;
    case 0x19:
        actor->node19 = (DWORD *)node;
        break;
    case 0x3a:
        actor->node3a = (DWORD *)node;
        break;
    case 0x44:
        actor->node44 = (DWORD *)node;
        break;
    }
    previous = successor->previous;
    node->next = successor;
    node->previous = previous;
    successor->previous = node;
    if (world->activeHead == successor)
        world->activeHead = successor;
    else
        node->previous->next = node;
    if (world->secondaryHead == successor)
        world->secondaryHead = node;
    world->activeCount++;
    return result;
}

// FUNCTION: EQGFX 0x10009237
// Stop an actor's scheduled updates and return its entry to the world.
// __cdecl. Unlinks the actor's list node from the world's active lists
// and pushes it onto the free list at +0x31c.
int __cdecl _t3dRemoveActiveAI(char *world, DWORD *actor)
{
    DWORD *node;

    switch (*actor)
    {
    case 0x18:
        node = (DWORD *)actor[0x17]; // +0x5c
        actor[0x17] = 0;
        break;
    case 0x19:
        node = (DWORD *)actor[0x0d]; // +0x34
        actor[0x0d] = 0;
        break;
    case 0x3a:
        node = (DWORD *)actor[0x0b]; // +0x2c
        actor[0x0b] = 0;
        break;
    case 0x44:
        node = (DWORD *)actor[9]; // +0x24
        actor[9] = 0;
        break;
    default:
        break;
    }
    if (*(DWORD **)(world + 0x314) == node)
        *(DWORD **)(world + 0x314) = (DWORD *)node[1];
    else
        *(DWORD *)(node[0] + 4) = node[1];
    if (*(DWORD **)(world + 0x318) == node)
        *(DWORD **)(world + 0x318) = (DWORD *)node[0];
    else
        *(DWORD *)node[1] = node[0];
    if (*(DWORD **)(world + 0x320) == node)
        *(DWORD **)(world + 0x320) = (DWORD *)node[1];
    node[1] = *(DWORD *)(world + 0x31c);
    node[0] = 0;
    node[2] = 0;
    (*(DWORD *)(world + 0x310))--;
    *(DWORD **)(world + 0x31c) = node;
    return 0;
}

// FUNCTION: EQGFX 0x100092e3
// Release the world's actor update-list storage during world teardown.
// __cdecl. Frees each 0x1ff-entry block chained at world+0x324 via +0x27f0,
// clearing the actor link slot selected by the actor type tag.
int __cdecl t3dFreeActorNodePool_guess(char *world)
{
    char *block;
    DWORD *slot;
    DWORD *actor;
    int count;
    short done;

    block = *(char **)(world + 0x324);
    done = 0;
    do
    {
        if (*(char **)(block + 0x27f0) == 0)
            done = 1;
        else
            world = *(char **)(block + 0x27f0);
        slot = (DWORD *)(block + 8);
        count = 0x1ff;
        do
        {
            actor = (DWORD *)*slot;
            if (actor != 0)
            {
                switch (*actor)
                {
                case 0x18:
                    actor[0x17] = 0;
                    break;
                case 0x19:
                    actor[0x0d] = 0;
                    break;
                case 0x3a:
                    actor[0x0b] = 0;
                    break;
                case 0x44:
                    actor[9] = 0;
                    break;
                default:
                    break;
                }
            }
            slot += 5;
            count--;
        } while (count != 0);
        EQ_CDECL(void, 0x100681DB, (void *))(block);
        if (done != 0)
            break;
        block = world;
    } while (1);
    return 0;
}

// FUNCTION: EQGFX 0x1000935f
// Allocate and prepare another block of actor-list entries for the world.
int __cdecl t3dAllocateActorNodePoolBlock_guess(T3dWorldActorRefSet_guess *world)
{
    int result;
    T3dActorRefSetBlock_guess *block;
    T3dActorRefSetSlot_guess *slot;
    int slot_count;

    result = 0;
    block = (T3dActorRefSetBlock_guess *)EQGFX_T3D_ALLOCATE_MEMORY(0x2800);
    ((T3dActorRefSetHolder_guess *)world->actorRefSetState)->block = block;
    block = ((T3dActorRefSetHolder_guess *)world->actorRefSetState)->block;
    if (block == 0)
    {
        result = 0xffff3d41;
    }
    else
    {
        slot = block->slots;
        slot_count = 0x1ff;
        do
        {
            slot->field00 = &slot->field10;
            slot->field08 = 0;
            slot->field0c = 0;
            slot++;
            slot_count--;
        } while (slot_count != 0);
        block->next = 0;
        block->field27f8 = 0;
        block->field27fc = 0;
        world->actorRefSetState = block;
    }
    return result;
}

// FUNCTION: EQGFX 0x100093d1
// Creates a named actor definition (template actor + shared sprite table), or returns the one already defined.
int *__cdecl t3dDefineActor_guess(
    void *dictionary,
    char *name,
    int entry_count,
    T3dActorDataEntry_guess *entries,
    int (__cdecl *create_callback)(void *, void *, void *),
    int resourceData,
    DWORD *initialValues,
    DWORD volumeA,
    DWORD volumeB,
    DWORD actor_field_34,
    DWORD actor_field_38,
    int dataEntryOffset,
    void *spritePointers,
    int mark_sprite_flag)
{
    int *existing_entry;
    T3dActorDefinitionObject_guess *definition;
    T3D_tagACTORINSTANCE *actor;
    T3dActorDataEntry_guess *entry_cursor;
    DWORD *parameter_cursor;
    int sprite_count;
    int loop_count;
    int callback_result;
    int sprite_bytes;
    size_t name_bytes;
    float default_channel_value;
    float *bounds_cursor;
    T3dActorSpriteObject_guess **sprite_cursor;
    T3dActorSpriteObject_guess **sprite_end;
    T3dActorSpriteObject_guess **unshare_cursor;
    T3dActorSpriteObject_guess *sprite;
    DWORD sprite_kind;

    existing_entry = (int *)EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(dictionary, name);
    if (existing_entry != 0)
    {
        if (*existing_entry == T3D_KIND_ACTOR_DEFINITION)
        {
            return existing_entry;
        }
        return 0;
    }

    definition = (T3dActorDefinitionObject_guess *)EQGFX_T3D_ALLOCATE_MEMORY(0x2c);
    if (definition == 0)
    {
        return 0;
    }

    definition->kind = T3D_KIND_ACTOR_DEFINITION;
    *(DWORD *)&definition->flags = 0;
    definition->state = 1;
    definition->entry_count = entry_count;
    if (create_callback == 0)
    {
        create_callback = EQGFX_T3D_AI_STUB_CALLBACK;
        definition->destroy_callback = create_callback;
    }
    else
    {
        definition->destroy_callback = create_callback;
    }
    definition->field24 = resourceData;
    name_bytes = EQGFX_STRLEN(name) + 1;
    definition->dictionary_key = EQGFX_T3D_ALLOCATE_MEMORY((int)name_bytes);
    if (definition->dictionary_key == 0)
    {
        EQGFX_T3D_FREE_MEMORY(definition);
    }
    else
    {
        EQGFX_T3D_COPY_STRING((char *)definition->dictionary_key, name);
        callback_result = EQGFX__T3D_ADD_ITEM_TO_LIST(dictionary, definition);
        if (callback_result == 0)
        {
            callback_result = EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY(dictionary, definition);
            if (callback_result < 0)
            {
                EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, definition);
                EQGFX_T3D_FREE_MEMORY(definition->dictionary_key);
                EQGFX_T3D_FREE_MEMORY(definition);
            }
            else
            {
                definition->entries = entries;
                entry_cursor = entries + entry_count;
                sprite_count = 0;
                while (entries < entry_cursor)
                {
                    entry_cursor--;
                    sprite_count += entry_cursor->field00;
                }
                definition->sprite_count = sprite_count;
                actor = (T3D_tagACTORINSTANCE *)EQGFX_T3D_ALLOCATE_MEMORY(0x114);
                if (actor != 0)
                {
                    definition->actor = actor;
                    actor->kind_guess = T3D_KIND_ACTOR;
                    actor->dictionaryKey_guess = 0;
                    *(DWORD *)&actor->flags_guess[0] = mark_sprite_flag == 0 ? 0 : 0x80000000;
                    parameter_cursor = actor->initialValues_guess;
                    loop_count = 7;
                    do
                    {
                        *parameter_cursor = *initialValues;
                        initialValues++;
                        parameter_cursor++;
                        loop_count--;
                    } while (loop_count != 0);
                    actor->scaleFactor_bits_guess = actor_field_34;
                    actor->boundingRadius_bits_guess = actor_field_38;
                    actor->currentAction_guess = dataEntryOffset;
                    actor->collisionVolume_guess = 0;
                    actor->renderVolume_guess = 0;
                    actor->unk_0x48 = 0;
                    actor->unk_0x4C = 0;
                    actor->ownedBuffer_guess = 0;
                    actor->dataEntryEnd_guess = entries + dataEntryOffset;
                    actor->light_guess = 0;
                    actor->unk_0x58 = 0;
                    actor->activeAI_guess = 0;
                    actor->userData = 0;
                    bounds_cursor = actor->scaledRotationMatrix_guess;
                    for (loop_count = 9; loop_count != 0; loop_count--)
                    {
                        bounds_cursor[-9] = 0.0f;
                        *bounds_cursor = 0.0f;
                        bounds_cursor[9] = 0.0f;
                        bounds_cursor++;
                    }
                    ((BYTE *)&actor->flags_guess[0])[3] |= T3D_ACTOR_FLAG_HIGH_BYTE;
                    actor->collisionVolume_guess = (void *)volumeA;
                    default_channel_value = t3d_float_ratio_max_guess;
                    actor->colorD4_guess = default_channel_value;
                    actor->colorD8_guess = default_channel_value;
                    actor->colorDC_guess = default_channel_value;
                    actor->colorE8_guess = default_channel_value;
                    actor->colorEC_guess = default_channel_value;
                    actor->spriteCacheIndex_guess |= 0xffffffff;
                    actor->unk_0xE4 |= 0xffffffff;
                    actor->colorF0_guess = default_channel_value;
                    actor->renderVolume_guess = (void *)volumeB;
                    actor->definition_guess = (T3dActorSpriteGroup_guess *)definition;

                    sprite_bytes = sprite_count * 4;
                    actor->spriteTable_guess = (T3dActorSpriteObject_guess **)EQGFX_T3D_ALLOCATE_MEMORY(sprite_bytes);
                    if (actor->spriteTable_guess == 0)
                    {
                        EQGFX_T3D_FREE_MEMORY(actor);
                        EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY(dictionary, name);
                        EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, definition);
                        EQGFX_T3D_FREE_MEMORY(definition->dictionary_key);
                        EQGFX_T3D_FREE_MEMORY(definition);
                    }
                    else
                    {
                        EQGFX_MEMCPY(actor->spriteTable_guess, spritePointers, sprite_bytes);
                        actor->frameStamp_guess = 0;
                        actor->collisionStatus_bits_guess = 0;
                        callback_result = EQGFX__T3D_ADD_ITEM_TO_LIST(dictionary, actor);
                        if (callback_result == 0)
                        {
                            sprite_end = actor->spriteTable_guess + sprite_count;
                            sprite_cursor = sprite_end;
                            while (actor->spriteTable_guess < sprite_cursor)
                            {
                                sprite_cursor--;
                                if (*sprite_cursor == 0)
                                {
                                    *sprite_cursor = EQGFX_DEFAULT_ACTOR_SPRITE;
                                }
                                sprite = *sprite_cursor;
                                sprite_kind = sprite->kind;
                                if (sprite_kind < 0x13)
                                {
                                    if (sprite_kind == 0x12)
                                    {
                                        goto share_sprite;
                                    }
                                    if (sprite_kind != 0)
                                    {
                                        if (sprite_kind != 0x06 && sprite_kind != 0x08 && sprite_kind != 0x0a)
                                        {
                                            if (sprite_kind != 0x0c)
                                            {
                                                goto invalid_sprite;
                                            }
                                            if (mark_sprite_flag != 0)
                                            {
                                                sprite->flags |= 0x80;
                                            }
                                        }
                                        goto share_sprite;
                                    }
                                }
                                else if (sprite_kind != 0x14 && sprite_kind != 0x48
                                    && sprite_kind != 0x4c && sprite_kind != 0x51)
                                {
                                    goto invalid_sprite;
                                }
                                else
                                {
                                    goto share_sprite;
                                }
                                goto next_sprite;

share_sprite:
                                EQGFX_T3D_SHARE_OBJECT(sprite);
next_sprite:
                                ;
                            }
                            callback_result = definition->destroy_callback(
                                EQGFX_DAT_10086118, dictionary, definition);
                            if (callback_result >= 0)
                            {
                                return (int *)definition;
                            }
                            sprite_cursor = actor->spriteTable_guess + sprite_count;
                            while (actor->spriteTable_guess < sprite_cursor)
                            {
                                sprite_cursor--;
                                sprite = *sprite_cursor;
                                sprite_kind = sprite->kind;
                                if (sprite_kind < 0x13)
                                {
                                    if (sprite_kind == 0x12 || sprite_kind == 0x06
                                        || sprite_kind == 0x08 || sprite_kind == 0x0a
                                        || sprite_kind == 0x0c)
                                    {
                                        EQGFX_T3D_UNSHARE_OBJECT(sprite);
                                    }
                                }
                                else if (sprite_kind == 0x14 || sprite_kind == 0x48
                                    || sprite_kind == 0x4c || sprite_kind == 0x51)
                                {
                                    EQGFX_T3D_UNSHARE_OBJECT(sprite);
                                }
                            }
                        }
                        else
                        {
                            EQGFX_T3D_FREE_MEMORY(actor->spriteTable_guess);
                            EQGFX_T3D_FREE_MEMORY(actor);
                            EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY(dictionary, name);
                            EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, definition);
                            EQGFX_T3D_FREE_MEMORY(definition->dictionary_key);
                            EQGFX_T3D_FREE_MEMORY(definition);
                            return 0;
                        }

                        EQGFX_T3D_FREE_MEMORY(actor->spriteTable_guess);
                        EQGFX_T3D_FREE_MEMORY(actor);
                        EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY(dictionary, name);
                        EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, definition);
                        EQGFX_T3D_FREE_MEMORY(definition->dictionary_key);
                        EQGFX_T3D_FREE_MEMORY(definition);
                    }
                }
                else
                {
                    EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY(dictionary, name);
                    EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, definition);
                    EQGFX_T3D_FREE_MEMORY(definition->dictionary_key);
                    EQGFX_T3D_FREE_MEMORY(definition);
                }
            }
        }
        else
        {
            EQGFX_T3D_FREE_MEMORY(definition->dictionary_key);
            EQGFX_T3D_FREE_MEMORY(definition);
        }
    }
    return 0;

invalid_sprite:
    unshare_cursor = actor->spriteTable_guess + sprite_count - 1;
    while (unshare_cursor > sprite_cursor)
    {
        sprite = *unshare_cursor;
        sprite_kind = sprite->kind;
        if (sprite_kind < 0x13)
        {
            if (sprite_kind == 0x12 || sprite_kind == 0x06
                || sprite_kind == 0x08 || sprite_kind == 0x0a
                || sprite_kind == 0x0c)
            {
                EQGFX_T3D_UNSHARE_OBJECT(sprite);
            }
        }
        else if (sprite_kind == 0x14 || sprite_kind == 0x48
            || sprite_kind == 0x4c || sprite_kind == 0x51)
        {
            EQGFX_T3D_UNSHARE_OBJECT(sprite);
        }
        unshare_cursor--;
    }
    EQGFX_T3D_FREE_MEMORY(actor->spriteTable_guess);
    EQGFX_T3D_FREE_MEMORY(actor);
    EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY(dictionary, name);
    EQGFX_T3D_FREE_OBJECT_POOL_SLOT(dictionary, definition);
    EQGFX_T3D_FREE_MEMORY(definition->dictionary_key);
    EQGFX_T3D_FREE_MEMORY(definition);
    return 0;
}

// FUNCTION: EQGFX 0x100097f5
// Unload an actor definition and its instantiated actor from the world.
int __cdecl t3dUndefineActor(void *world, T3dActorDefinitionObject_guess *actorDefinition)
{
    int result;
    T3D_tagACTORINSTANCE *actor;
    T3dActorSpriteObject_guess **sprite_end;
    T3dActorSpriteObject_guess *sprite_object;
    T3dActorDataEntry_guess *entry_end;
    int dictionary_result;

    result = 0;
    if (actorDefinition->state != 1)
    {
        result = T3D_ERROR_INVALID_ACTOR_STATE;
    }
    else
    {
        actorDefinition->destroy_callback(&EQGFX_DAT_1008611C, world, actorDefinition);
        actor = actorDefinition->actor;
        if (actor->activeAI_guess != 0)
        {
            EQ_CDECL(void, 0x10009237, (void *, void *))(world, actor);
        }
        if (actor->light_guess != 0)
        {
            EQGFX_T3D_DESTROY_POINT_LIGHT(world, actor->light_guess);
        }
        if ((actor->flags_guess[0] & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(world, actor->dictionaryKey_guess);
        }
        EQ_CDECL(int, 0x10015D81, (void *, void *))(world, actor);
        sprite_end = actor->spriteTable_guess + actorDefinition->sprite_count;
        if (actor->spriteTable_guess < sprite_end)
        {
            do
            {
                sprite_end--;
                sprite_object = *sprite_end;
                switch (sprite_object->kind)
                {
                case T3D_KIND_COMPOSITE_SPRITE:
                    EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                    EQ_CDECL(void, 0x1000D6E5, (void *, void *))(world, *sprite_end);
                    break;
                case 0:
                    break;
                case T3D_KIND_SIMPLE_SPRITE:
                    EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                    EQ_CDECL(void, 0x1001B94A, (void *, void *))(world, *sprite_end);
                    break;
                case T3D_KIND_BLIT_SPRITE:
                    EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                    EQ_CDECL(void, 0x1000D06A, (void *, void *))(world, *sprite_end);
                    break;
                case T3D_KIND_2D_SPRITE:
                    EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                    EQ_CDECL(void, 0x10007D99, (void *, void *))(world, *sprite_end);
                    break;
                case T3D_KIND_3D_SPRITE:
                    EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                    EQ_CDECL(void, 0x1000879D, (void *, void *))(world, *sprite_end);
                    break;
                case T3D_KIND_HIERARCHICAL_SPRITE:
                    EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                    EQ_CDECL(void, 0x100132A2, (void *, void *))(world, *sprite_end);
                    break;
                case T3D_KIND_DM_SPRITE:
                    EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                    EQ_CDECL(void, 0x10010575, (void *, void *))(world, *sprite_end);
                    break;
                case 0x4c:
                    EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                    break;
                case T3D_KIND_STRING_SPRITE:
                    EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                    EQ_CDECL(void, 0x1001ACF8, (void *, void *))(world, *sprite_end);
                    break;
                default:
                    result = T3D_ERROR_UNSUPPORTED_ACTOR_SPRITE;
                    break;
                }
            } while (actor->spriteTable_guess < sprite_end);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(actor->spriteTable_guess);
        EQ_CDECL(void, 0x1001D5A1, (void *, void *))(world, actor->renderVolume_guess);
        EQ_CDECL(void, 0x1001D5A1, (void *, void *))(world, actor->collisionVolume_guess);
        if (actor->ownedBuffer_guess != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(actor->ownedBuffer_guess);
        }
        if (actor->dictionaryKey_guess != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(actor->dictionaryKey_guess);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(actor);
        if ((actorDefinition->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(world, actorDefinition->dictionary_key);
        }
        dictionary_result = EQ_CDECL(int, 0x10015D81, (void *, void *))(world, actorDefinition);
        if (result == 0)
        {
            result = dictionary_result;
        }
        entry_end = actorDefinition->entries + actorDefinition->entry_count;
        if (actorDefinition->entries < entry_end)
        {
            do
            {
                entry_end--;
                EQ_CDECL(void, 0x10050CBB, (void *))(entry_end->buffer);
            } while (actorDefinition->entries < entry_end);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(actorDefinition->entries);
        if (actorDefinition->dictionary_key != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(actorDefinition->dictionary_key);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(actorDefinition);
    }
    return result;
}

// FUNCTION: EQGFX 0x10009A18
// Decrements the shared count at +0x10; always returns 0.
int __cdecl t3dUnshare2DSprite(T3dShared_guess *sprite)
{
    sprite->ref_count--;
    return 0;
}

// FUNCTION: EQGFX 0x10009A22
// Builds an actor from its definition and unwinds owned resources on any failed step.
T3D_tagACTORINSTANCE *__cdecl t3dCreateActorEx(void *context, T3dActorDefinitionObject_guess *actorDefinition,
                                          const char *name, const DWORD *placement, void *volumeASource,
                                          RenderVolume_guess *volumeBSource, float radius, float scale,
                                          int markActor, DWORD userData)
{
    unsigned int processedCount;
    unsigned int copyCount;
    unsigned int kind;
    int shareSprite;
    DWORD *copyFrom;
    DWORD *copyTo;
    T3D_tagACTORINSTANCE *sourceActor;
    T3D_tagACTORINSTANCE *actor;
    RenderVolume_guess *volume;
    T3dActorSpriteObject_guess **spriteSlot;
    T3dActorSpriteObject_guess **allocatedSprite;
    T3dActorSpriteObject_guess **sourceSpriteSlot;
    T3dActorSpriteObject_guess *sprite;
    void *particleCloud;

    processedCount = 0;
    actor = (T3D_tagACTORINSTANCE *)EQGFX_T3D_ALLOCATE_MEMORY(0x114);
    if (actor == 0)
    {
        return 0;
    }
    sourceActor = actorDefinition->actor;
    copyFrom = (DWORD *)sourceActor;
    copyTo = (DWORD *)actor;
    for (copyCount = 0x45; copyCount != 0; copyCount--)
    {
        *copyTo = *copyFrom;
        copyFrom++;
        copyTo++;
    }
    actor->ambient_guess = 0.0f;
    actor->listIndex_guess = -1;
    actor->spriteCacheIndex_guess = 0xffffffff;
    *((DWORD *)&actor->flags_guess[0]) = (*((DWORD *)&actor->flags_guess[0]) & T3D_ACTOR_FLAGS_PRESERVED_MASK) |
                                T3D_ACTOR_FLAGS_CREATE;
    actor->userData = userData;
    actor->activeAI_guess = 0;
    actor->dmRgbTrack = 0;
    actor->particleCount_guess = 0;
    actor->unk_0x104 = 0;
    actor->particleClouds_guess = 0;
    actor->renderVolume_guess = 0;
    actor->collisionVolume_guess = 0;
    actor->unk_0x58 = 0;
    actor->ownedBuffer_guess = 0;
    actor->spriteTable_guess = 0;
    actor->surfacePitchType = 0;
    actor->dictionaryKey_guess = 0;
    if (name != 0)
    {
        actor->dictionaryKey_guess = EQGFX_T3D_ALLOCATE_MEMORY((int)(EQGFX_STRLEN(name) + 1));
        if (actor->dictionaryKey_guess == 0)
        {
            goto cleanup;
        }
        EQGFX_T3D_COPY_STRING((char *)actor->dictionaryKey_guess, name);
    }
    if (placement != 0)
    {
        copyFrom = (DWORD *)placement;
        copyTo = &actor->initialValues_guess[0];
        for (copyCount = 7; copyCount != 0; copyCount--)
        {
            *copyTo = *copyFrom;
            copyFrom++;
            copyTo++;
        }
    }
    actor->regionIndex_guess = EQGFX_T3D_GET_REGION_NUMBER(context, &actor->initialValues_guess[0]);
    if (actor->regionIndex_guess == -1)
    {
        goto cleanup;
    }
    if (volumeASource == 0)
    {
        volumeASource = sourceActor->collisionVolume_guess;
    }
    actor->collisionVolume_guess = EQGFX_T3D_DUPLICATE_VOLUME(context, volumeASource);
    if (actor->collisionVolume_guess == 0)
    {
        goto cleanup;
    }
    volume = (RenderVolume_guess *)actor->collisionVolume_guess;
    if (volume->volumeType == 4)
    {
        volume->owningActor = actor;
    }
    if (volumeBSource == 0)
    {
        actor->renderVolume_guess = 0;
    }
    else
    {
        actor->renderVolume_guess = EQGFX_T3D_DUPLICATE_VOLUME(context, volumeBSource);
        if (actor->renderVolume_guess == 0)
        {
            goto cleanup;
        }
        volume = (RenderVolume_guess *)actor->renderVolume_guess;
        if (radius == EQGFX_ZERO_F_GUESS)
        {
            if (volume->volumeType == T3D_KIND_VOLUME_RADIUS)
            {
                radius = volume->radius_guess;
                scale = radius;
            }
            else if (volume->volumeType == T3D_KIND_VOLUME_RADIUS_ALT)
            {
                radius = volume->sphylinderRadius_guess;
                scale = radius;
            }
        }
    }
    if (radius == EQGFX_ZERO_F_GUESS)
    {
        radius = *(float *)&sourceActor->scaleFactor_bits_guess;
    }
    if (scale == EQGFX_ZERO_F_GUESS)
    {
        scale = *(float *)&sourceActor->boundingRadius_bits_guess;
    }
    *(float *)&actor->boundingRadius_bits_guess = scale;
    *(float *)&actor->scaleFactor_bits_guess = radius;
    if (sourceActor->light_guess == 0)
    {
        actor->light_guess = 0;
    }
    else
    {
        actor->light_guess = EQGFX_T3D_DUPLICATE_POINT_LIGHT_EX(context, sourceActor->light_guess,
                                                               placement, 1);
        if (actor->light_guess == 0)
        {
            goto cleanup;
        }
    }
    actor->unk_0x58 = 0;
    if ((actor->flags_guess[3] & 0x80) == 0)
    {
        actor->unk_0x4C = 8;
        actor->ownedBuffer_guess = EQGFX_T3D_ALLOCATE_MEMORY(0x20);
        if (actor->ownedBuffer_guess == 0)
        {
            goto cleanup;
        }
    }
    else
    {
        actor->unk_0x4C = 0;
        actor->ownedBuffer_guess = 0;
    }
    actor->unk_0x48 = 0;
    actor->spriteTable_guess = (T3dActorSpriteObject_guess **)EQGFX_T3D_ALLOCATE_MEMORY(
        actorDefinition->sprite_count << 2);
    if (actor->spriteTable_guess == 0)
    {
        goto cleanup;
    }
    EQGFX_MEMCPY(actor->spriteTable_guess, sourceActor->spriteTable_guess, actorDefinition->sprite_count << 2);
    if (EQGFX__T3D_ADD_ITEM_TO_LIST(context, actor) != 0)
    {
        goto cleanup;
    }
    if (markActor != 0)
    {
        actor->flags_guess[3] |= 0x10;
    }
    if ((actor->dictionaryKey_guess == 0 ||
         EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(context, (const char *)actor->dictionaryKey_guess) != 0 ||
         EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY(context, actor) >= 0) &&
        EQGFX_T3D_UPDATE_TOUCHED_REGIONS(context, actor, actor->regionIndex_guess) >= 0)
    {
        processedCount = 0;
        spriteSlot = actor->spriteTable_guess;
        if (actorDefinition->sprite_count != 0)
        {
            do
            {
                sprite = *spriteSlot;
                kind = sprite->kind;
                if (kind < 0x13)
                {
                    if (kind == T3D_KIND_COMPOSITE_SPRITE)
                    {
                        goto shareObject;
                    }
                    if (kind != 0)
                    {
                        if (kind != T3D_KIND_SIMPLE_SPRITE && kind != T3D_KIND_BLIT_SPRITE &&
                            kind != T3D_KIND_2D_SPRITE)
                        {
                            shareSprite = kind == T3D_KIND_3D_SPRITE;
                            if (shareSprite == 0)
                            {
                                goto cleanup;
                            }
                            goto shareObject;
                        }
                        goto shareObject;
                    }
                }
                else
                {
                    if (kind != T3D_KIND_HIERARCHICAL_SPRITE && kind != T3D_KIND_DM_SPRITE)
                    {
                        if (kind == T3D_KIND_PARTICLE_CLOUD_SOURCE)
                        {
                            particleCloud = EQGFX_T3D_CREATE_PARTICLE_CLOUD(
                                context, (DWORD)sprite, (int)&actor->initialValues_guess[0], 0, 0);
                            *spriteSlot = (T3dActorSpriteObject_guess *)particleCloud;
                            if (particleCloud != 0)
                            {
                                EQGFX_T3D_BIND_PARTICLE_CLOUD_SPAWN(particleCloud, actor);
                                goto nextSprite;
                            }
                            goto cleanup;
                        }
                        shareSprite = kind == T3D_KIND_STRING_SPRITE;
                        if (shareSprite == 0)
                        {
                            goto cleanup;
                        }
                    }
                    goto shareObject;
                }
                goto nextSprite;
shareObject:
                EQGFX_T3D_SHARE_OBJECT(sprite);
nextSprite:
                processedCount++;
                spriteSlot++;
            } while (processedCount < (unsigned int)actorDefinition->sprite_count);
        }
        if (actorDefinition->destroy_callback(EQGFX_DAT_10086114_ADDRESS, context, actor) >= 0)
        {
            actorDefinition->state++;
            return actor;
        }
    }
cleanup:
    allocatedSprite = actor->spriteTable_guess;
    if (allocatedSprite != 0 && processedCount != 0)
    {
        sourceSpriteSlot = sourceActor->spriteTable_guess;
        do
        {
            kind = (*sourceSpriteSlot)->kind;
            if (kind < 0x13)
            {
                if (kind != T3D_KIND_COMPOSITE_SPRITE && kind != T3D_KIND_SIMPLE_SPRITE &&
                    kind != T3D_KIND_BLIT_SPRITE && kind != T3D_KIND_2D_SPRITE)
                {
                    shareSprite = kind == T3D_KIND_3D_SPRITE;
                    if (shareSprite == 0)
                    {
                        goto nextCleanupSprite;
                    }
                }
            }
            else
            {
                if (kind == T3D_KIND_HIERARCHICAL_SPRITE || kind == T3D_KIND_DM_SPRITE)
                {
                    goto unshareObject;
                }
                if (kind != T3D_KIND_PARTICLE_CLOUD_SOURCE)
                {
                    shareSprite = kind == T3D_KIND_STRING_SPRITE;
                    if (shareSprite == 0)
                    {
                        goto nextCleanupSprite;
                    }
                    goto unshareObject;
                }
                EQGFX_T3D_DESTROY_PARTICLE_CLOUD(context, allocatedSprite);
                goto nextCleanupSprite;
            }
unshareObject:
            EQGFX_T3D_UNSHARE_OBJECT(*allocatedSprite);
nextCleanupSprite:
            sourceSpriteSlot++;
            allocatedSprite++;
            processedCount--;
        } while (processedCount != 0);
    }
    if (actor->ownedBuffer_guess != 0)
    {
        EQGFX_T3D_FREE_MEMORY(actor->ownedBuffer_guess);
    }
    if (actor->light_guess != 0)
    {
        EQGFX_T3D_DESTROY_POINT_LIGHT(context, actor->light_guess);
    }
    if (actor->collisionVolume_guess != 0)
    {
        EQGFX_T3D_DESTROY_VOLUME(context, actor->collisionVolume_guess);
    }
    if (actor->renderVolume_guess != 0)
    {
        EQGFX_T3D_DESTROY_VOLUME(context, actor->renderVolume_guess);
    }
    if (actor->listIndex_guess != -1)
    {
        EQGFX_T3D_FREE_OBJECT_POOL_SLOT(context, actor);
    }
    if ((actor->flags_guess[0] & 1) != 0)
    {
        EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY(context, (const char *)actor->dictionaryKey_guess);
    }
    if (actor->dictionaryKey_guess != 0)
    {
        EQGFX_T3D_FREE_MEMORY(actor->dictionaryKey_guess);
    }
    if (actor->particleClouds_guess != 0)
    {
        EQGFX_T3D_FREE_MEMORY(actor->particleClouds_guess);
    }
    EQGFX_T3D_FREE_MEMORY(actor);
    return 0;
}

// FUNCTION: EQGFX 0x10009E60
// Copies source into a new actor with its own volumes, light and sprite table; returns it, or 0 after undoing each step.
T3D_tagACTORINSTANCE *__cdecl t3dDuplicateActorEx(void *context, T3D_tagACTORINSTANCE *source, const char *name,
                                            const DWORD *placement, void *volumeASource, RenderVolume_guess *volumeBSource,
                                            float radius, float scale, DWORD userData)
{
    unsigned int processedCount;
    T3D_tagACTORINSTANCE *duplicate;
    T3dActorSpriteGroup_guess *spriteGroup;
    T3dActorSpriteObject_guess **slot;
    T3dActorSpriteObject_guess *sprite;
    void *particleCloud;
    const DWORD *copyFrom;
    DWORD *copyTo;
    unsigned int copyCount;
    unsigned int kind;

    processedCount = 0;
    duplicate = (T3D_tagACTORINSTANCE *)EQGFX_T3D_ALLOCATE_MEMORY(0x114);
    if (duplicate == 0)
    {
        return 0;
    }
    copyFrom = (const DWORD *)source;
    copyTo = (DWORD *)duplicate;
    for (copyCount = 0x45; copyCount != 0; copyCount--)
    {
        *copyTo++ = *copyFrom++;
    }
    duplicate->listIndex_guess = -1;
    duplicate->spriteCacheIndex_guess = -1;
    *(DWORD *)&duplicate->flags_guess[0] = (*(DWORD *)&duplicate->flags_guess[0] & 0xe0000000) | (T3D_ACTOR_FLAG_HIGH_BYTE << 24);
    duplicate->userData = userData;
    duplicate->activeAI_guess = 0;
    duplicate->dmRgbTrack = 0;
    duplicate->particleCount_guess = 0;
    duplicate->unk_0x104 = 0;
    duplicate->particleClouds_guess = 0;
    duplicate->renderVolume_guess = 0;
    duplicate->collisionVolume_guess = 0;
    duplicate->unk_0x58 = 0;
    duplicate->ownedBuffer_guess = 0;
    duplicate->spriteTable_guess = 0;
    duplicate->dictionaryKey_guess = 0;
    if (name != 0)
    {
        duplicate->dictionaryKey_guess = EQGFX_T3D_ALLOCATE_MEMORY(EQGFX_STRLEN(name) + 1);
        if (duplicate->dictionaryKey_guess == 0)
        {
            goto cleanup;
        }
        EQGFX_T3D_COPY_STRING(duplicate->dictionaryKey_guess, name);
    }
    if (placement != 0)
    {
        copyFrom = placement;
        copyTo = duplicate->initialValues_guess;
        for (copyCount = 7; copyCount != 0; copyCount--)
        {
            *copyTo++ = *copyFrom++;
        }
    }
    duplicate->regionIndex_guess = EQGFX_T3D_GET_REGION_NUMBER(context, duplicate->initialValues_guess);
    if (duplicate->regionIndex_guess == -1)
    {
        goto cleanup;
    }
    spriteGroup = duplicate->definition_guess;
    duplicate->collisionVolume_guess = EQGFX_T3D_DUPLICATE_VOLUME(context, (volumeASource == 0) ? source->collisionVolume_guess : volumeASource);
    if (duplicate->collisionVolume_guess == 0)
    {
        goto cleanup;
    }
    if (volumeBSource == 0)
    {
        if (source->renderVolume_guess == 0)
        {
            duplicate->renderVolume_guess = 0;
        }
        else
        {
            duplicate->renderVolume_guess = EQGFX_T3D_DUPLICATE_VOLUME(context, source->renderVolume_guess);
        }
    }
    else
    {
        duplicate->renderVolume_guess = EQGFX_T3D_DUPLICATE_VOLUME(context, volumeBSource);
        if (radius == 0.0)
        {
            switch (volumeBSource->volumeType)
            {
            case 0x2a:
                scale = radius = volumeBSource->radius_guess;
                break;
            case 0x2c:
                scale = radius = volumeBSource->sphylinderRadius_guess;
                break;
            }
        }
    }
    if (radius == 0.0)
    {
        radius = *(float *)&source->scaleFactor_bits_guess;
    }
    *(float *)&duplicate->scaleFactor_bits_guess = radius;
    if (scale == 0.0)
    {
        scale = *(float *)&source->boundingRadius_bits_guess;
    }
    *(float *)&duplicate->boundingRadius_bits_guess = scale;
    if (source->light_guess == 0)
    {
        duplicate->light_guess = 0;
    }
    else
    {
        duplicate->light_guess = EQGFX_T3D_DUPLICATE_POINT_LIGHT_EX(context, source->light_guess, placement, 1);
        if (duplicate->light_guess == 0)
        {
            goto cleanup;
        }
    }
    duplicate->unk_0x58 = 0;
    duplicate->spriteTable_guess = (T3dActorSpriteObject_guess **)EQGFX_T3D_ALLOCATE_MEMORY(spriteGroup->sprite_count << 2);
    if (duplicate->spriteTable_guess == 0)
    {
        goto cleanup;
    }
    EQGFX_MEMCPY(duplicate->spriteTable_guess, source->spriteTable_guess, spriteGroup->sprite_count << 2);
    if ((duplicate->flags_guess[3] & 0x80) == 0)
    {
        duplicate->unk_0x4C = 8;
        duplicate->ownedBuffer_guess = EQGFX_T3D_ALLOCATE_MEMORY(0x20);
        if (duplicate->ownedBuffer_guess == 0)
        {
            goto cleanup;
        }
    }
    else
    {
        duplicate->unk_0x4C = 0;
        duplicate->ownedBuffer_guess = 0;
    }
    duplicate->unk_0x48 = 0;
    if (EQGFX__T3D_ADD_ITEM_TO_LIST(context, duplicate) == 0 &&
        (duplicate->dictionaryKey_guess == 0 ||
         EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(context, duplicate->dictionaryKey_guess) != 0 ||
         EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY(context, duplicate) >= 0) &&
        EQGFX_T3D_UPDATE_TOUCHED_REGIONS(context, duplicate, duplicate->regionIndex_guess) >= 0)
    {
        processedCount = 0;
        if (spriteGroup->sprite_count != 0)
        {
            slot = duplicate->spriteTable_guess;
            do
            {
                sprite = *slot;
                kind = sprite->kind;
                if (kind < 0x13)
                {
                    if (kind == T3D_KIND_COMPOSITE_SPRITE)
                    {
                        goto share;
                    }
                    if (kind != 0)
                    {
                        if (kind != T3D_KIND_SIMPLE_SPRITE && kind != T3D_KIND_BLIT_SPRITE &&
                            kind != T3D_KIND_2D_SPRITE)
                        {
                            if (kind != T3D_KIND_3D_SPRITE)
                            {
                                goto cleanup;
                            }
                        }
                        goto share;
                    }
                }
                else
                {
                    if (kind != T3D_KIND_HIERARCHICAL_SPRITE && kind != T3D_KIND_DM_SPRITE)
                    {
                        if (kind == T3D_KIND_PARTICLE_CLOUD)
                        {
                            particleCloud = EQGFX_T3D_CREATE_PARTICLE_CLOUD(context,
                                ((T3dParticleCloudSprite_guess *)sprite)->field11c, 0, 0, 0);
                            *slot = (T3dActorSpriteObject_guess *)particleCloud;
                            if (particleCloud == 0)
                            {
                                goto cleanup;
                            }
                            EQGFX_T3D_BIND_PARTICLE_CLOUD_SPAWN(particleCloud, duplicate);
                            goto next;
                        }
                        if (kind != T3D_KIND_STRING_SPRITE)
                        {
                            goto cleanup;
                        }
                    }
share:
                    EQGFX_T3D_SHARE_OBJECT(sprite);
                }
next:
                processedCount++;
                slot++;
            } while (processedCount < (unsigned int)spriteGroup->sprite_count);
        }
        if (spriteGroup->destroy_callback(&EQGFX_DAT_10086114, context, duplicate) >= 0)
        {
            spriteGroup->ref_count++;
            return duplicate;
        }
    }
cleanup:
    if (duplicate->spriteTable_guess != 0)
    {
        for (slot = duplicate->spriteTable_guess; processedCount != 0; processedCount--)
        {
            sprite = *slot;
            kind = sprite->kind;
            if (kind < 0x13)
            {
                if (kind == T3D_KIND_COMPOSITE_SPRITE || kind == T3D_KIND_SIMPLE_SPRITE ||
                    kind == T3D_KIND_BLIT_SPRITE || kind == T3D_KIND_2D_SPRITE || kind == T3D_KIND_3D_SPRITE)
                {
                    EQGFX_T3D_UNSHARE_OBJECT(sprite);
                }
            }
            else if (kind == T3D_KIND_HIERARCHICAL_SPRITE || kind == T3D_KIND_DM_SPRITE ||
                     kind == T3D_KIND_STRING_SPRITE)
            {
                EQGFX_T3D_UNSHARE_OBJECT(sprite);
            }
            else if (kind == T3D_KIND_PARTICLE_CLOUD)
            {
                EQGFX_T3D_DESTROY_PARTICLE_CLOUD(context, slot);
            }
            slot++;
        }
    }
    if (duplicate->ownedBuffer_guess != 0)
    {
        EQGFX_T3D_FREE_MEMORY(duplicate->ownedBuffer_guess);
    }
    if (duplicate->light_guess != 0)
    {
        EQGFX_T3D_DESTROY_POINT_LIGHT(context, duplicate->light_guess);
    }
    if (duplicate->collisionVolume_guess != 0)
    {
        EQGFX_T3D_DESTROY_VOLUME(context, duplicate->collisionVolume_guess);
    }
    if (duplicate->renderVolume_guess != 0)
    {
        EQGFX_T3D_DESTROY_VOLUME(context, duplicate->renderVolume_guess);
    }
    if (duplicate->listIndex_guess != -1)
    {
        EQGFX_T3D_FREE_OBJECT_POOL_SLOT(context, duplicate);
    }
    if ((duplicate->flags_guess[0] & T3D_ACTOR_FLAG_IN_DICTIONARY) != 0)
    {
        EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY(context, duplicate->dictionaryKey_guess);
    }
    if (duplicate->dictionaryKey_guess != 0)
    {
        EQGFX_T3D_FREE_MEMORY(duplicate->dictionaryKey_guess);
    }
    if (duplicate->particleClouds_guess != 0)
    {
        EQGFX_T3D_FREE_MEMORY(duplicate->particleClouds_guess);
    }
    EQGFX_T3D_FREE_MEMORY(duplicate);
    return 0;
}

// FUNCTION: EQGFX 0x1000A25C
// Destroy an actor instance and release its effects, lights, sprites, and volumes.
int __cdecl t3dDestroyActor(void *world, T3D_tagACTORINSTANCE *actor)
{
    int result;
    T3dActorSpriteObject_guess **sprite;
    T3dActorSpriteObject_guess **sprite_end;
    int particle_count;
    void **particle;
    void *particle_cloud;

    actor->definition_guess->destroy_callback(&EQGFX_DAT_10086120, world, actor);
    EQ_CDECL(void, 0x10001091, (void *, void *))(world, actor);
    if (actor->activeAI_guess != 0)
    {
        EQ_CDECL(void, 0x10009237, (void *, void *))(world, actor);
    }
    actor->definition_guess->ref_count--;
    if (actor->light_guess != 0)
    {
        EQGFX_T3D_DESTROY_POINT_LIGHT(world, actor->light_guess);
    }
    if (actor->dmRgbTrack != 0)
    {
        EQ_CDECL(void, 0x10017DAD, (void *))(actor->dmRgbTrack);
        EQ_CDECL(void, 0x10010EED, (void *, void *))(world, actor->dmRgbTrack);
    }
    if (actor->ownedBuffer_guess != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(actor->ownedBuffer_guess);
    }
    EQ_CDECL(void, 0x1001D5A1, (void *, void *))(world, actor->renderVolume_guess);
    EQ_CDECL(void, 0x1001D5A1, (void *, void *))(world, actor->collisionVolume_guess);
    sprite_end = actor->spriteTable_guess + actor->definition_guess->sprite_count;
    sprite = sprite_end - 1;
    if (sprite_end > actor->spriteTable_guess)
    {
        T3dActorSpriteObject_guess *sprite_object;
        do
        {
            sprite_object = *sprite;
            switch (sprite_object->kind)
            {
            case T3D_KIND_COMPOSITE_SPRITE:
                EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                EQ_CDECL(void, 0x1000D6E5, (void *, void *))(world, *sprite);
                break;
            case 0:
                break;
            case T3D_KIND_SIMPLE_SPRITE:
                EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                EQ_CDECL(void, 0x1001B94A, (void *, void *))(world, *sprite);
                break;
            case T3D_KIND_BLIT_SPRITE:
                EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                EQ_CDECL(void, 0x1000D06A, (void *, void *))(world, *sprite);
                break;
            case T3D_KIND_2D_SPRITE:
                EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                EQ_CDECL(void, 0x10007D99, (void *, void *))(world, *sprite);
                break;
            case T3D_KIND_3D_SPRITE:
                EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                EQ_CDECL(void, 0x1000879D, (void *, void *))(world, *sprite);
                break;
            case T3D_KIND_HIERARCHICAL_SPRITE:
                EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                EQ_CDECL(void, 0x100132A2, (void *, void *))(world, *sprite);
                break;
            case T3D_KIND_DM_SPRITE:
                EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                EQ_CDECL(void, 0x10010575, (void *, void *))(world, *sprite);
                break;
            case T3D_KIND_PARTICLE_CLOUD:
                break;
            case T3D_KIND_STRING_SPRITE:
                EQ_CDECL(void, 0x10017DAD, (void *))(sprite_object);
                EQ_CDECL(void, 0x1001ACF8, (void *, void *))(world, *sprite);
                break;
            }
        } while (sprite-- > actor->spriteTable_guess);
    }
    EQ_CDECL(void, 0x10050CBB, (void *))(actor->spriteTable_guess);
    particle = actor->particleClouds_guess;
    for (particle_count = actor->particleCount_guess; particle_count != 0; particle_count--)
    {
        particle_cloud = *particle;
        if (particle_cloud != 0)
        {
            if (*(DWORD *)particle_cloud != T3D_KIND_PARTICLE_CLOUD)
            {
                particle++;
            }
            else
            {
                EQ_CDECL(void, 0x10040217, (void *, void **))(world, particle);
            }
        }
    }
    if (actor->particleClouds_guess != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(actor->particleClouds_guess);
    }
    if ((actor->flags_guess[0] & T3D_ACTOR_FLAG_IN_DICTIONARY) != 0)
    {
        EQ_CDECL(void, 0x1000E334, (void *, void *))(world, actor->dictionaryKey_guess);
    }
    result = EQ_CDECL(int, 0x10015D81, (void *, void *))(world, actor);
    if (actor->dictionaryKey_guess != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(actor->dictionaryKey_guess);
    }
    EQ_CDECL(void, 0x10050CBB, (void *))(actor);
    return result;
}

// FUNCTION: EQGFX 0x1000A453
int __cdecl t3dAddObjectToActorRefSet(T3dActorRefSet_guess *referenceSet, int object)
{
    int result;
    unsigned int ref_count;
    unsigned int index;
    int *ref_list;
    unsigned int new_capacity;

    result = 0;
    index = 0;
    ref_count = referenceSet->objectCount;
    if (ref_count != 0)
    {
        ref_list = referenceSet->objects;
        do
        {
            if (*ref_list == object)
                return 0;
            index++;
            ref_list++;
        } while (index < ref_count);
    }
    if (ref_count == referenceSet->objectCapacity)
    {
        if (referenceSet->objects == 0)
        {
            new_capacity = T3D_ACTOR_REF_SET_GROWTH;
            ref_list = EQGFX_T3D_ALLOCATE_MEMORY(0x14);
        }
        else
        {
            new_capacity = referenceSet->objectCapacity + T3D_ACTOR_REF_SET_GROWTH;
            ref_list = EQGFX_T3D_REALLOCATE_MEMORY(referenceSet->objects, new_capacity * 4);
        }
        referenceSet->objects = ref_list;
        if (ref_list == 0)
            result = (int)0xFFFF3D3E;
        else
            referenceSet->objectCapacity = new_capacity;
    }
    if (result == 0)
    {
        referenceSet->objects[ref_count] = object;
        referenceSet->objectCount++;
    }
    return result;
}

// FUNCTION: EQGFX 0x1000a4ed
// __cdecl (the callee pops nothing; ECX is not an input). Removes `object`
// from the actor's reference set: the count at +0x100 over the int array at
// +0x108. On a hit the tail shifts down one (memcpy) unless it was the last
// entry, the count at +0x100 drops by one and 0 returns; otherwise 0xFFFF3D3E.
int __cdecl t3dRemoveObjectFromActorRefSet(T3dActorRefSet_guess *referenceSet, int object)
{
    unsigned int ref_count;
    unsigned int index;
    int *ref_list;
    int *current;

    index = 0;
    ref_count = referenceSet->objectCount;
    if (ref_count > 0)
    {
        ref_list = referenceSet->objects;
        current = ref_list;
        do
        {
            if (*current != object)
            {
                index++;
                current++;
            }
            else
            {
                if (index != ref_count - 1)
                    EQGFX_MEMCPY(ref_list + index, ref_list + index + 1,
                                 ((index + 1) * 0x3fffffff + ref_count) * 4);
                referenceSet->objectCount--;
                return 0;
            }
        } while (index < ref_count);
    }
    return 0xFFFF3D3E;
}

// FUNCTION: EQGFX 0x1000A553
// Forward a message to the actor's sprite-group handler.
// __cdecl with three stack arguments, naked: the asm is the whole function and ends with its own RET, as the
// original does (no prologue, no locals). Forwards its arguments to the handler at *(*(object + 0x64) + 0x20):
// pushes object, data, message (the handler sees message, data, object), loads the object
// *(object + 0x64) into ECX as the handler's this, and pops the three arguments itself after the call (the
// handler does not pop). EAX is not touched, so the handler's return value comes back unchanged.
EQ_NAKED DWORD __cdecl t3dSendMessageToActor_guess(DWORD data, int object, DWORD message)
{
    __asm {
        mov eax, dword ptr [esp + 0x8]
        push eax
        push dword ptr [esp + 0x8]
        mov ecx, dword ptr [eax + 0x64]
        push dword ptr [esp + 0x14]
        call dword ptr [ecx + 0x20]
        add esp, 0xc
        ret
    }
}

// FUNCTION: EQGFX 0x1000a56a
// Schedule an inactive actor for updates at the requested position.
// __cdecl with four stack arguments (callee pops 0 bytes): returns unless the active node for the actor's kind
// is NULL, then passes the actor to 0x10009031, 0x1000917a, 0x100090b8 or 0x10008f98 by placement (1..4).
void __cdecl t3dActivateAI(DWORD world, T3dActiveUpdate_guess *actor, int placement, T3dActiveUpdate_guess *insertBefore)
{
    BOOL inactive;

    switch (actor->kind)
    {
    case 0x18:
        inactive = (actor->node18 == 0);
        break;
    case 0x19:
        inactive = (actor->node19 == 0);
        break;
    case 0x3a:
        inactive = (actor->node3a == 0);
        break;
    case 0x44:
        inactive = (actor->node44 == 0);
        break;
    default:
        return;
    }
    if (inactive)
    {
        switch (placement)
        {
        case 1:
            EQGFX_t3dPrependActorToActiveList(world, actor);
            break;
        case 2:
            EQGFX_T3D_INSERT_ACTOR_BEFORE_IN_ACTIVE_LIST(world, insertBefore, actor);
            break;
        case 3:
            EQGFX_T3D_INSERT_ACTOR_AFTER_IN_ACTIVE_LIST(world, insertBefore, actor);
            break;
        case 4:
            EQGFX_T3D_APPEND_ACTOR_TO_ACTIVE_LIST(world, actor);
            break;
        }
    }
}

// FUNCTION: EQGFX 0x1000a5e0
// Stops an actor's scheduled AI updates.
// __cdecl with two stack arguments: returns when the active node for kind is
// already NULL, otherwise unlinks it through 0x10009237.
void __cdecl t3dDeactivateAI(DWORD world, T3dActiveUpdate_guess *actor)
{
    switch (actor->kind)
    {
    case 0x18:
        if (actor->node18 == 0)
            return;
        break;
    case 0x19:
        if (actor->node19 == 0)
            return;
        break;
    case 0x3a:
        if (actor->node3a == 0)
            return;
        break;
    case 0x44:
        if (actor->node44 == 0)
            return;
        break;
    default:
        return;
    }
    EQ_CDECL(void, 0x10009237, (DWORD, T3dActiveUpdate_guess *))(world, actor);
}

// FUNCTION: EQGFX 0x1000a61d
// __cdecl with two stack arguments: stores the time at +0xc of the node picked by kind.
DWORD __cdecl t3dSetActiveAIUpdateTime(T3dActiveUpdate_guess *actor, DWORD updateTime)
{
    DWORD *target;

    switch (actor->kind)
    {
    case 0x18:
        target = actor->node18;
        break;
    case 0x19:
        target = actor->node19;
        break;
    case 0x3a:
        target = actor->node3a;
        break;
    case 0x44:
        target = actor->node44;
        break;
    default:
        return 0xFFFF3D3E;
    }
    if (target == 0)
        return 0xFFFF3D3E;
    target[3] = updateTime;
    return 0;
}

// FUNCTION: EQGFX 0x1000a65b
// Provides access to custom data attached to an actor.
// __cdecl with one stack argument (callee pops 0 bytes): actor block holding
// the user data pointer at +0x60. Returns the actor's user data.
DWORD __cdecl t3dGetActorUserData(DWORD *actor)
{
    return actor[0x18]; // +0x60
}

// FUNCTION: EQGFX 0x1000a663
// Lets other systems read an actor's world position and orientation.
// __cdecl with two stack arguments (callee pops 0 bytes): copies the 0x1c
// bytes at actor+0x10 into the output buffer. Returns 0.
DWORD __cdecl t3dGetActorLocation(DWORD *actor, void *outLocation)
{
    memcpy(outLocation, (BYTE *)actor + 0x10, 0x1c);
    return 0;
}

// The first argument is the world (its region table pointer sits at +0x24), not an actor: only that member is
// declared here (the actor layout is T3D_tagACTORINSTANCE).
typedef struct T3dRegionOwner_guess
{
    DWORD fields000[9];
    DWORD *regionList; // +0x24
} T3dRegionOwner_guess;

// FUNCTION: EQGFX 0x1000a67c
// Looks up the world region for an actor's current region index.
// __cdecl with two stack arguments (callee pops 0 bytes): returns
// regionList[selector->regionIndex] for the table at world+0x24.
DWORD __cdecl t3dGetActorCurrentRegion(T3dRegionOwner_guess *world, T3D_tagACTORINSTANCE *selector)
{
    return world->regionList[selector->regionIndex_guess];
}

// FUNCTION: EQGFX 0x1000a68e
// Gets a sprite from a hierarchical actor's specified group and entry.
// __cdecl with three stack arguments (callee pops 0 bytes): actor block whose
// DWORD at +0x0 is 0x18 for a hierarchical-sprite actor, the sprite table at
// +0x44 and the group block at +0x64. Returns spriteTable[slot] for
// groupIndex/spriteIndex, or 0 when the type tag or either bound check fails.
DWORD __cdecl t3dGetActorSprite(DWORD *actor, DWORD groupIndex, DWORD spriteIndex)
{
    DWORD foundSprite;
    DWORD *actorGroup;
    DWORD *groupEntry;
    DWORD *entryItems;
    DWORD tableSlot;

    foundSprite = 0;
    if (*actor == 0x18)
    {
        actorGroup = (DWORD *)actor[0x19]; // +0x64
        if (groupIndex < *(DWORD *)((BYTE *)actorGroup + 0x14))
        {
            groupEntry = (DWORD *)((BYTE *)*(DWORD **)((BYTE *)actorGroup + 0x18) + groupIndex * 8);
            if (spriteIndex < *groupEntry)
            {
                entryItems = (DWORD *)groupEntry[1];
                tableSlot = ((DWORD *)((BYTE *)entryItems + 8))[spriteIndex * 3];
                foundSprite = ((DWORD *)actor[0x11])[tableSlot]; // +0x44
            }
        }
    }
    return foundSprite;
}

// FUNCTION: EQGFX 0x1000a6c6
// Gets an entry's end-distance threshold; its exact game use is unclear.
float __cdecl t3dGetActorEndDistance(T3dActorEndDistanceActor_guess *actor, DWORD groupIndex, DWORD itemIndex)
{
    float endDistance;
    T3dActorEndDistanceGroup_guess *actorGroup;
    T3dActorEndDistanceEntry_guess *groupEntry;

    endDistance = 0.0f;
    if (actor->kind == 0x18)
    {
        actorGroup = actor->distanceGroup;
        if (groupIndex < actorGroup->groupCount)
        {
            groupEntry = actorGroup->entries + groupIndex;
            if (itemIndex < groupEntry->itemCount)
                endDistance = EQGFX_T3D_FLOAT_SQRT(groupEntry->items[itemIndex].endDistanceSquared);
        }
    }
    return endDistance;
}

// FUNCTION: EQGFX 0x1000a702
// Identifies the collision-volume type associated with a sprite.
// __cdecl with one stack argument (callee pops 0 bytes): sprite block holding
// the collision volume block pointer at +0x2c; returns the volume type at +0x0.
DWORD __cdecl s3dGetDMSpriteCollisionVolumeType(DWORD *sprite)
{
    return *(DWORD *)sprite[0x0B];
}

// FUNCTION: EQGFX 0x1000a70c
// __cdecl with one stack argument: actor definition block holding the kind at +0x00.
DWORD __cdecl t3dIsActorDefinitionValid(T3dActorDefinition_guess *definition)
{
    DWORD result;

    result = 0;
    if (definition->kind != 0x17)
        result = 0xFFFF3D3E;
    return result;
}

// FUNCTION: EQGFX 0x1000a71d
// __cdecl with one stack argument: actor block holding the kind at +0x00.
DWORD __cdecl t3dIsActorValid(DWORD *actor)
{
    DWORD result;

    result = 0;
    if (*actor != 0x18)
        result = 0xFFFF3D3E;
    return result;
}

 // FUNCTION: EQGFX 0x1000a72e
 // __cdecl with two stack arguments (callee pops 0 bytes): actor block holding
 // the current action index at +0x3c, the current action entry pointer at +0x40
 // and the action group block at +0x64 (count at +0x14, entries at +0x18).
 // Returns 0xffff3d3e when actionIndex is out of range, else 0.
DWORD __cdecl t3dSetActorCurrentAction(DWORD *actor, DWORD actionIndex)
{
    DWORD *actorGroup;

    actorGroup = (DWORD *)actor[0x19]; // +0x64
    if (actionIndex >= *(DWORD *)((BYTE *)actorGroup + 0x14))
        return 0xFFFF3D3E;
    actor[0x0F] = actionIndex; // +0x3c
    actor[0x10] = *(DWORD *)((BYTE *)actorGroup + 0x18) + actionIndex * 8; // +0x40
    return 0;
}

// FUNCTION: EQGFX 0x1000a756
// __cdecl setter storing the actor user-data pointer at +0x60, returns 0.
DWORD __cdecl t3dSetActorUserData(DWORD *actor, DWORD userData)
{
    actor[0x18] = userData;
    return 0;
}

typedef struct T3dActorLocation_guess
{
    float position[3];
    float orientation[3];
    DWORD extra;
} T3dActorLocation_guess;

#define T3D_ACTOR_FLAG_MOVED 0x08
#define T3D_ACTOR_FLAG_ORIENTATION_CHANGED 0x20

// FUNCTION: EQGFX 0x1000a764
// Moves and reorients an actor in the world.
int __cdecl t3dSetActorLocation(void *world, T3D_tagACTORINSTANCE *actor, T3dActorLocation_guess *location)
{
    float *position;
    int result;
    int oldRegion;
    int region;
    int lightResult;

    position = (float *)&actor->initialValues_guess[0];
    result = 0;
    if (position[0] == location->position[0] && position[1] == location->position[1] && position[2] == location->position[2])
    {
        if (location->orientation[0] != position[3] || location->orientation[1] != position[4] || location->orientation[2] != position[5])
            actor->flags_guess[3] |= T3D_ACTOR_FLAG_ORIENTATION_CHANGED;
        position[3] = location->orientation[0];
        position[4] = location->orientation[1];
        position[5] = location->orientation[2];
    }
    else
    {
        oldRegion = actor->regionIndex_guess;
        if (location->orientation[0] != position[3] || location->orientation[1] != position[4] || location->orientation[2] != position[5])
            actor->flags_guess[3] |= T3D_ACTOR_FLAG_ORIENTATION_CHANGED;
        actor->flags_guess[3] |= T3D_ACTOR_FLAG_MOVED;
        region = EQ_CDECL(int, 0x100192C4, (void *, T3dActorLocation_guess *))(world, location);
        if (region == -1)
        {
            actor->regionIndex_guess = oldRegion;
        }
        else
        {
            EQGFX_MEMCPY(position, location, 0x1c);
            actor->regionIndex_guess = region;
            if (EQ_CDECL(int, 0x10001000, (void *, T3D_tagACTORINSTANCE *, int))(world, actor, oldRegion) >= 0)
            {
                if (actor->light_guess != 0)
                {
                    lightResult = EQ_CDECL(int, 0x100145BB, (void *, void *, float *))(world, actor->light_guess, position);
                    if (lightResult != 0)
                        result = lightResult;
                }
                return result;
            }
        }
        result = 0xffff3d3e;
    }
    return result;
}

// FUNCTION: EQGFX 0x1000a86b
// Changes the sprite used for one group entry of a hierarchical actor.
DWORD __cdecl t3dSetActorSprite(T3D_tagACTORINSTANCE *actor, unsigned int groupIndex, unsigned int spriteIndex, T3dActorSpriteObject_guess *sprite)
{
    T3dActorSpriteGroup_guess *actorGroup;
    T3dActorSpriteEntry_guess *groupEntry;
    T3dActorSpriteItem_guess *spriteItem;
    T3dActorSpriteObject_guess *oldSprite;
    DWORD tableIndex;
    int objectKind;
    int isSupported;

    if (actor->kind_guess != 0x18)
        return 0xFFFF3D3E;
    actorGroup = actor->definition_guess;
    if (groupIndex >= actorGroup->count)
        return 0xFFFF3D3E;
    groupEntry = &actorGroup->entries[groupIndex];
    if (spriteIndex >= groupEntry->count)
        return 0xFFFF3D3E;
    spriteItem = &groupEntry->items[spriteIndex];
    tableIndex = spriteItem->tableIndex;
    oldSprite = actor->spriteTable_guess[tableIndex];
    objectKind = oldSprite->kind;
    if (objectKind < 0x13)
    {
        if (objectKind == 0x12)
            goto unshareOldSprite;
        if (objectKind != 0)
        {
            if (((objectKind != 6) && (objectKind != 8)) && (objectKind != 10))
            {
                isSupported = objectKind == 0x0c;
                goto checkOldSpriteKind;
            }
            goto unshareOldSprite;
        }
    }
    else
    {
        if ((objectKind != 0x14) && (objectKind != 0x48))
        {
            if (objectKind == 0x4d)
                goto installSprite;
            isSupported = objectKind == 0x51;
            goto checkOldSpriteKind;
        }
        goto unshareOldSprite;
    }
    goto installSprite;

checkOldSpriteKind:
    if (!isSupported)
        return 0xFFFF3D58;

unshareOldSprite:
    EQGFX_T3D_UNSHARE_OBJECT(oldSprite);

installSprite:
    if (sprite == 0)
        sprite = EQGFX_DEFAULT_ACTOR_SPRITE;
    actor->spriteTable_guess[tableIndex] = sprite;
    objectKind = sprite->kind;
    if (objectKind < 0x0d)
    {
        if (objectKind != 0x0c)
        {
            if (objectKind == 0)
                goto finish;
            if ((objectKind != 6) && (objectKind != 8))
            {
                isSupported = objectKind == 10;
                goto checkNewSpriteKind;
            }
        }
    }
    else if (objectKind != 0x12)
    {
        if (objectKind == 0x14)
        {
            EQGFX_T3D_SHARE_OBJECT(sprite);
            sprite->ownerActor = actor;
            goto finish;
        }
        if (objectKind != 0x48)
        {
            isSupported = objectKind == 0x51;
            goto checkNewSpriteKind;
        }
    }
    EQGFX_T3D_SHARE_OBJECT(sprite);
    goto finish;

checkNewSpriteKind:
    if (!isSupported)
        return 0xFFFF3D58;
    EQGFX_T3D_SHARE_OBJECT(sprite);

finish:
    actor->spriteCacheIndex_guess = 0xFFFFFFFF;
    return 0;
}

// FUNCTION: EQGFX 0x1000a959
// __cdecl bounds-checked setter storing the squared end distance.
DWORD __cdecl t3dSetActorEndDistance(DWORD *actor, unsigned int groupIndex, unsigned int itemIndex, float endDistance)
{
    T3dActorDistanceGroup_guess *actorGroup;
    T3dActorDistanceEntry_guess *groupEntry;
    DWORD itemCount;
    DWORD result;

    result = 0xFFFF3D3E;
    if (*actor == 0x18)
    {
        actorGroup = (T3dActorDistanceGroup_guess *)actor[0x19]; // +0x64
        if (groupIndex < actorGroup->count)
        {
            itemCount = actorGroup->entries[groupIndex].count;
            groupEntry = &actorGroup->entries[groupIndex];
            itemCount--;
            if (itemIndex < itemCount)
            {
                groupEntry->items[itemIndex].endDistance = endDistance * endDistance;
                result = 0;
            }
        }
    }
    return result;
}
