// unit_100205AE.c - eqgfx_dx7.dll, the address run 0x100205AE..0x10021897 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

typedef struct T3dZoneActorEntry_guess T3dZoneActorEntry_guess;

typedef struct T3dZonePointLightArray_guess
{
    char pad[0x10];
    unsigned int count; // +0x10
    unsigned int capacity; // +0x14
    void **lights; // +0x18
} T3dZonePointLightArray_guess;

typedef struct T3dZoneActorOwner_guess
{
    char pad[0x28];
    T3dZoneActorEntry_guess *actor; // +0x28
} T3dZoneActorOwner_guess;

typedef struct T3dZoneActorEntry_guess
{
    char pad0[0x28];
    unsigned int regionIndex; // +0x28
    T3dZonePointLightArray_guess *pointLightArray; // +0x2c
    char pad1[0x34];
    T3dZoneActorOwner_guess *owner; // +0x64
} T3dZoneActorEntry_guess;

typedef struct T3dZoneReferenceList_guess
{
    char pad0[4];
    unsigned int count; // +0x4
    char pad1[4];
    T3dZoneActorEntry_guess **actors; // +0xc
} T3dZoneReferenceList_guess;

typedef struct T3dZoneActorList_guess
{
    char pad0[0x24];
    void **regionReferences; // +0x24
    char pad1[0x14c];
    unsigned int actorCount; // +0x174
    char pad2[4];
    T3dZoneActorEntry_guess **actors; // +0x17c
    char pad3[0x1c4];
    T3dZoneReferenceList_guess referenceList; // +0x344, reference list header
    unsigned int *pointLightRegionIndices; // +0x354
} T3dZoneActorList_guess;

typedef struct ZoneRegions_guess
{
    char pad[8];
    void *dictionaryKey; // +0x08
    unsigned char flags; // +0x0c, bit 0: dictionaryKey is in the dictionary
    char pad3[3]; // +0x0d
    char referenceList[4]; // +0x10, reference list header (its +0x4 count and +0xc items are regionCount and regionList)
    int regionCount; // +0x14
    char pad2[4]; // +0x18
    int *regionList; // +0x1c
    void *extraBlock; // +0x20
} ZoneRegions_guess;

typedef struct T3dZoneRegionIndex_guess
{
    char pad[4];
    unsigned int regionIndex; // +0x4
} T3dZoneRegionIndex_guess;

typedef struct FileHandle_guess
{
    void *fileRecord; // +0x00, non-zero while the file is open
} FileHandle_guess;

typedef struct FastFileStream_guess
{
    char pad[8];
    FileHandle_guess *handle; // +0x08
} FastFileStream_guess;

typedef struct FastFile_guess
{
    FastFileStream_guess *stream; // +0x00
} FastFile_guess;

typedef struct t3dRegionSet_guess
{
    int tag; // +0x00, 0x27 and 0x29 hold a reference list at +0x28
    int entryCount; // +0x04, count of the int list at +0x0c (read for every tag by t3dGetActorsTouchingZone)
    char pad0[4]; // +0x08
    int *entries; // +0x0c
    int objectCount; // +0x10, count of the pointer list at +0x18 (read by t3dGetPointLightsInfluencingZone)
    char pad1[4]; // +0x14
    int **objects; // +0x18, pointers to objects whose first dword is a type tag
    char pad[0xc]; // +0x1c
    char referenceList[4]; // +0x28, reference list header (its +0x4 count and +0xc items are referenceCount and referenceItems)
    int referenceCount; // +0x2c
    char pad2[4]; // +0x30
    int *referenceItems; // +0x34
} t3dRegionSet_guess;

typedef struct t3dWorldObject_guess
{
    char pad[0x2c];
    t3dRegionSet_guess *regions; // +0x2c
} t3dWorldObject_guess;

typedef struct PointLight_guess
{
    char pad0[0x0f];
    unsigned char flags;
    char pad1[4];
    char position[12]; // +0x14, xyz (3 values assumed), passed by address to t3dIsWorldXYZContainedInZone
    char pad2[4];
    int entryCount;
    unsigned int entryCapacity; // +0x28
    int *entries;
} PointLight_guess;

typedef struct T3D_tagACTORINSTANCE
{
    char pad[0x24];
    t3dWorldObject_guess **zones; // +0x24, zone table indexed by position->regionIndex
} T3D_tagACTORINSTANCE;

typedef struct T3dPosition_guess
{
    char pad[0x28];
    int regionIndex; // +0x28
    char pad2[0x1c]; // +0x2c
    int extraRegionCount; // +0x48
    char pad3[4]; // +0x4c
    int *extraRegionIndices; // +0x50, indices into actor->zones
} T3dPosition_guess;

typedef struct t3dWorld_guess
{
    char pad[0x24];
    int *objectTable; // +0x24, indexed by a light's entry numbers
    char pad2[0x19c]; // +0x28
    unsigned int pointLightEntryCount; // +0x1c4
    char pad3a[4]; // +0x1c8
    PointLight_guess **pointLightEntries; // +0x1cc
    char pad3b[0x74]; // +0x1d0
    unsigned int zoneCount; // +0x244
    char pad4[4]; // +0x248
    ZoneRegions_guess **zones; // +0x24c, zone table indexed by the zone loop
} t3dWorld_guess;

typedef struct DirectionalLight_guess
{
    char pad[0x24];
    int entryCount; // +0x24
    unsigned int entryCapacity; // +0x28
    int *entries; // +0x2c, indices into the world's object table
} DirectionalLight_guess;

typedef struct t3dLight_guess
{
    char pad[0x14];
    int entryCount; // +0x14
    char pad2[4]; // +0x18
    t3dWorldObject_guess **entries; // +0x1c, t3dWorldObject_guess pointers
} t3dLight_guess;

typedef struct FastFileRefCounted_guess
{
    void *vtable; // +0x00, slot 0 is the virtual deleting destructor
    int referenceCount; // +0x04
} FastFileRefCounted_guess;

typedef struct FastFileRef_guess
{
    FastFileRefCounted_guess *object; // +0x00, reference-counted, may be null
    void *extra; // +0x04
} FastFileRef_guess;

typedef struct FilePath_guess
{
    unsigned int words[2];
} FilePath_guess;

typedef struct FastFileInitLocals_guess
{
    FilePath_guess resultPath; // +0x00, [ebp-0xc]
    void *packFileSystemState; // +0x08, [ebp-0x4]
} FastFileInitLocals_guess;

typedef struct FfSharedRef_guess
{
    void *object;
    void *extra;
} FfSharedRef_guess;

typedef struct FastFileOpenReferenceStorage_guess
{
    unsigned char prefix[4];
    FastFileRefCounted_guess *temporaryObject;
} FastFileOpenReferenceStorage_guess;

typedef union FastFileOpenPathTemporary_guess
{
    FilePath_guess filePath;
    FastFileOpenReferenceStorage_guess referenceStorage;
} FastFileOpenPathTemporary_guess;

typedef struct FastFileOpenTemporaries_guess
{
    unsigned char prefix[4];
    FfSharedRef_guess sharedReference;
    FastFileOpenPathTemporary_guess pathAndTemporary;
    unsigned char reservedStack[12];
} FastFileOpenTemporaries_guess;

// FUNCTION: EQGFX 0x100205AE
// Destroys a zone grouping and removes its associations with world regions.
// __cdecl. Unlinks the zone from its regions' reference lists, then frees it. Returns t3dFreeObjectPoolSlot_guess's result, or 0xffff3d3e.
int __cdecl t3dDestroyZone(void *context, ZoneRegions_guess *zone)
{
    int *regionCursor;
    int *regionEnd;
    t3dWorldObject_guess *region;
    t3dRegionSet_guess *regions;
    void *referenceList;
    int referenceIndex;
    int result;

    regionCursor = zone->regionList;
    regionEnd = regionCursor + zone->regionCount;
    while (regionCursor < regionEnd)
    {
        region = (t3dWorldObject_guess *)*regionCursor;
        regions = region->regions;
        switch (regions->tag)
        {
        case 0x27:
        case 0x29:
            referenceList = &regions->referenceList;
            if (EQ_CDECL(int, 0x100190B6, (void *, void *, int *))(zone, referenceList, &referenceIndex) == 1 &&
                EQ_CDECL(int, 0x10019179, (int, void *))(referenceIndex, referenceList) != 0)
            {
                return 0xffff3d3e;
            }
            break;
        }
        regionCursor++;
    }
    if ((zone->flags & 1) != 0)
    {
        EQ_CDECL(void, 0x1000E334, (void *, void *))(context, zone->dictionaryKey);
    }
    result = EQ_CDECL(int, 0x10015D81, (void *, void *))(context, zone);
    EQ_CDECL(void, 0x100191BD, (void *))(&zone->referenceList);
    if (zone->dictionaryKey != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(zone->dictionaryKey);
    }
    if (zone->extraBlock != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(zone->extraBlock);
    }
    EQ_CDECL(void, 0x10050CBB, (void *))(zone);
    return result;
}

// FUNCTION: EQGFX 0x10020666
// Set the frame-skip count used when processing a light track.
// __cdecl (callee pops 0 bytes; ECX is not an input). Stores the skip-frame
// count at +0x20 of the object. Returns 0.
int __cdecl t3dSetLightSkipFrames(char *object, int skipFrames)
{
    *(int *)(object + 0x20) = skipFrames;
    return 0;
}

// FUNCTION: EQGFX 0x10020674
// Link a region and zone so each can find the other.
int __cdecl t3dAddRegionToZone(int unused, t3dWorldObject_guess *region, int zone)
{
    t3dRegionSet_guess *regions;
    ZoneRegions_guess *zoneObject;
    char *regionReferenceList;
    char *zoneReferenceList;
    int lookupResult;

    regions = region->regions;
    switch (regions->tag)
    {
    case 0x27:
    case 0x29:
        zoneObject = (ZoneRegions_guess *)zone;
        regionReferenceList = regions->referenceList;
        zoneReferenceList = zoneObject->referenceList;
        lookupResult = EQ_CDECL(int, 0x100190B6, (void *, void *, int *))(zoneObject, regionReferenceList, &zone);
        if (lookupResult == 0 && EQ_CDECL(int, 0x100190F9, (void *, void *, int))(zoneObject, regionReferenceList, zone) != 0)
        {
            return 0;
        }
        lookupResult = EQ_CDECL(int, 0x100190B6, (void *, void *, int *))(region, zoneReferenceList, &zone);
        if (lookupResult == 0)
        {
            EQ_CDECL(int, 0x100190F9, (void *, void *, int))(region, zoneReferenceList, zone);
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x100206E1
// Unlink a region from a zone.
// __cdecl (callee pops 0 bytes; ECX is not an input), int (always 0). For a region tagged 0x27 or 0x29, unlinks the zone
// from the region's zone list and the region from the zone's region list. The region argument is overwritten by the list index.
int __cdecl t3dRemoveRegionFromZone(int unused, int region, ZoneRegions_guess *zone)
{
    t3dWorldObject_guess *regionObject;
    t3dRegionSet_guess *regions;
    void *regionReferenceList;
    void *zoneReferenceList;

    regionObject = (t3dWorldObject_guess *)region;
    regions = regionObject->regions;
    switch (regions->tag)
    {
    case 0x27:
    case 0x29:
        regionReferenceList = &regions->referenceList;
        zoneReferenceList = &zone->referenceList;
        if (EQ_CDECL(int, 0x100190B6, (void *, void *, int *))(zone, regionReferenceList, &region) == 1 &&
            EQ_CDECL(int, 0x10019179, (int, void *))(region, regionReferenceList) == -0xc2c2)
        {
            return 0;
        }
        if (EQ_CDECL(int, 0x100190B6, (void *, void *, int *))(regionObject, zoneReferenceList, &region) == 1)
        {
            EQ_CDECL(int, 0x10019179, (int, void *))(region, zoneReferenceList);
        }
        break;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1002074B
// Find zones containing the region at an actor's current position.
// __cdecl (callee pops 0 bytes; ECX is not an input), int. Returns 0 if t3dGetZonesContainingRegion returns 0 for the zone at actor->zones[position->regionIndex], else 0xFFFF3D3E.
int __cdecl t3dGetZonesContainingActorPosition(T3D_tagACTORINSTANCE *actor, T3dPosition_guess *position, int region)
{
    t3dWorldObject_guess *zone;
    int result;

    result = 0;
    zone = actor->zones[position->regionIndex];
    if (EQ_CDECL(int, 0x100207AE, (void *, t3dWorldObject_guess *, int))(actor, zone, region) != 0)
    {
        result = 0xFFFF3D3E;
    }
    return result;
}

// FUNCTION: EQGFX 0x1002077A
// Find zones containing the region at a world-space position.
// __cdecl (callee pops 0 bytes; ECX is not an input), int. Gets the region at world/xyz; returns 0 if one exists and t3dGetZonesContainingRegion returns 0 for it, else 0xFFFF3D3E.
int __cdecl t3dGetZonesContainingWorldXYZPosition(int world, int xyz, void *zones)
{
    t3dWorldObject_guess *region;
    int result;

    result = 0;
    region = EQ_CDECL(t3dWorldObject_guess *, 0x10019324, (int, int))(world, xyz);
    if (region == 0 || EQ_CDECL(int, 0x100207AE, (int, t3dWorldObject_guess *, void *))(world, region, zones) != 0)
    {
        result = 0xFFFF3D3E;
    }
    return result;
}

// FUNCTION: EQGFX 0x100207AE
// Find the zones that contain a given region.
// __cdecl (callee pops 0 bytes; ECX is not an input), int. For a region tagged 0x27 or 0x29, clears zones and adds each
// item of the region's reference list to it; returns 0, or 0xFFFF3D3E when the tag is other or an add fails.
int __cdecl t3dGetZonesContainingRegion(int unused, t3dWorldObject_guess *region, void *zones)
{
    t3dRegionSet_guess *regions;
    int *entry;
    int *entryEnd;
    int result;

    result = 0;
    regions = region->regions;
    switch (regions->tag)
    {
    case 0x27:
    case 0x29:
        EQ_CDECL(void, 0x1001905A, (void *))(zones);
        entry = regions->referenceItems;
        entryEnd = entry + regions->referenceCount;
        for (; entry < entryEnd; entry++)
        {
            if (EQ_CDECL(int, 0x10019065, (int, void *))(*entry, zones) != 0)
            {
                goto failed;
            }
        }
        break;
    default:
    failed:
        result = 0xffff3d3e;
        break;
    }
    return result;
}

// FUNCTION: EQGFX 0x100207FD
// __cdecl. Returns t3dIsRegionContainedInZone's result for the zone at actor->zones[position->regionIndex].
int __cdecl t3dIsActorPositionContainedInZone(T3D_tagACTORINSTANCE *actor, T3dPosition_guess *position, int region)
{
    t3dWorldObject_guess *zone;

    zone = actor->zones[position->regionIndex];
    return EQ_CDECL(int, 0x1002084F, (void *, t3dWorldObject_guess *, int))(actor, zone, region);
}

// FUNCTION: EQGFX 0x1002081D
// __cdecl. Returns 1 if the zone at world/xyz (t3dGetRegionFromWorldAndXYZ) contains region, else 0.
int __cdecl t3dIsWorldXYZContainedInZone(int world, int xyz, int region)
{
    int contained;
    int zone;

    contained = 1;
    zone = EQ_CDECL(int, 0x10019324, (int, int))(world, xyz);
    if (zone == 0 || EQ_CDECL(int, 0x1002084F, (int, int, int))(world, zone, region) == 0)
    {
        contained = 0;
    }
    return contained;
}

// FUNCTION: EQGFX 0x1002084F
// __cdecl. Returns 1 if region is in worldObject's reference list (tags 0x27 and 0x29), else 0.
int __cdecl t3dIsRegionContainedInZone(int unused, t3dWorldObject_guess *worldObject, int region)
{
    t3dRegionSet_guess *regions;
    int contained;

    contained = 0;
    regions = worldObject->regions;
    switch (regions->tag)
    {
    case 0x27:
    case 0x29:
        if (EQ_CDECL(int, 0x100190B6, (int, void *, void *))(region, regions->referenceList, &worldObject) == 1)
        {
            contained = 1;
        }
        break;
    }
    return contained;
}

// FUNCTION: EQGFX 0x10020885
// List the regions contained in a zone.
// __cdecl. Clears referenceList, then adds each region of the zone to it; returns 0, or 0xFFFF3D3E when adding one fails.
int __cdecl t3dGetRegionsContainedInZone(int unused, ZoneRegions_guess *zone, void *referenceList)
{
    int result;
    int *regionCursor;
    int *regionEnd;

    result = 0;
    EQ_CDECL(void, 0x1001905A, (void *))(referenceList);
    regionCursor = zone->regionList;
    regionEnd = regionCursor + zone->regionCount;
    while (regionCursor < regionEnd)
    {
        if (EQ_CDECL(int, 0x10019065, (int, void *))(*regionCursor, referenceList) != 0)
        {
            result = 0xffff3d3e;
            break;
        }
        regionCursor++;
    }
    return result;
}

// FUNCTION: EQGFX 0x100208C6
// Find actors whose current region matches a specified zone region.
int __cdecl t3dGetActorsPositionedInZone(T3dZoneActorList_guess *zone, ZoneRegions_guess *region, void *referenceList)
{
    struct
    {
        int referenceMatch;
        int result;
    } locals;
    T3dZoneActorEntry_guess **actorCursor;
    T3dZoneActorEntry_guess **actorEnd;
    T3dZoneActorEntry_guess *actor;

    locals.result = 0;
    EQ_CDECL(void, 0x1001905A, (void *))(referenceList);
    actorCursor = zone->actors;
    actorEnd = actorCursor + zone->actorCount;
    while (1)
    {
        if (actorEnd <= actorCursor)
            return locals.result;
        actor = *actorCursor;
        if ((zone->actorCount < (unsigned int)actor) &&
            (actor->owner->actor != actor) &&
            (EQ_CDECL(int, 0x100190B6, (void *, void *, int *))(
                 zone->regionReferences[actor->regionIndex], &region->referenceList, &locals.referenceMatch) == 1) &&
            (EQ_CDECL(int, 0x10019065, (void *, void *))(actor, referenceList) != 0))
        {
            locals.result = 0xffff3d3e;
            break;
        }
        actorCursor++;
    }
    return locals.result;
}

// FUNCTION: EQGFX 0x10020955
// Send a message to actors in a specified zone region.
// __cdecl. Returns 0xffff3d3e when t3dGetActorsPositionedInZone returns nonzero, else sends message to each reference-list actor and returns 0.
int __cdecl t3dSendMessageToActorsPositionedInZone(T3dZoneActorList_guess *zone, ZoneRegions_guess *region, void *message)
{
    int result;
    T3dZoneReferenceList_guess *referenceList;
    T3dZoneActorEntry_guess **actorCursor;
    T3dZoneActorEntry_guess **actorEnd;

    result = 0;
    referenceList = &zone->referenceList;
    if (EQ_CDECL(int, 0x100208C6, (T3dZoneActorList_guess *, ZoneRegions_guess *, void *))(
            zone, region, referenceList) != 0)
    {
        result = 0xffff3d3e;
    }
    else
    {
        actorCursor = referenceList->actors;
        actorEnd = actorCursor + referenceList->count;
        for (; actorCursor < actorEnd; actorCursor++)
        {
            EQ_CDECL(void, 0x1000A553, (T3dZoneActorList_guess *, T3dZoneActorEntry_guess *, void *))(
                zone, *actorCursor, message);
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x100209AB
// Check whether an actor touches a zone through its current or extra regions.
// __cdecl. Returns 1 if region's reference list is in the zone at position->regionIndex or in the zone of any extra region index, else 0.
int __cdecl t3dIsActorTouchingZone(T3D_tagACTORINSTANCE *actor, T3dPosition_guess *position, ZoneRegions_guess *region)
{
    int touching;
    int *extraEntry;
    int *extraEnd;
    int referenceList;

    touching = 0;
    referenceList = (int)&region->referenceList;
    if (EQ_CDECL(int, 0x100190B6, (t3dWorldObject_guess *, int, ZoneRegions_guess **))(actor->zones[position->regionIndex],
            referenceList, &region) == 1)
    {
        goto found;
    }
    extraEntry = position->extraRegionIndices;
    extraEnd = extraEntry + position->extraRegionCount;
    while (extraEntry < extraEnd)
    {
        if (EQ_CDECL(int, 0x100190B6, (t3dWorldObject_guess *, int, ZoneRegions_guess **))(actor->zones[*extraEntry],
                referenceList, &region) == 1)
        {
            goto found;
        }
        extraEntry++;
    }
    goto done;
found:
    touching = 1;
done:
    return touching;
}

// FUNCTION: EQGFX 0x10020A20
// Lists all distinct zones touched by an actor across its primary and additional occupied regions.
int __cdecl t3dGetZonesTouchedByActor(T3D_tagACTORINSTANCE *actor, T3dPosition_guess *position, void *referenceList)
{
    int result;
    int insertIndex;
    int *extraRegionEnd;
    t3dRegionSet_guess *regions;
    int *referenceCursor;
    int *referenceEnd;
    int *extraRegionCursor;
    int reference;

    result = 0;
    EQ_CDECL(void, 0x1001905A, (void *))(referenceList);
    regions = actor->zones[position->regionIndex]->regions;
    referenceCursor = regions->referenceItems;
    referenceEnd = referenceCursor + regions->referenceCount;
    while (referenceCursor < referenceEnd)
    {
        reference = *referenceCursor;
        if (EQ_CDECL(int, 0x100190B6, (int, void *, int *))(reference, referenceList, &insertIndex) == 0 &&
                EQ_CDECL(int, 0x100190F9, (int, void *, int))(reference, referenceList, insertIndex) != 0)
        {
            goto insertion_failed;
        }
        referenceCursor++;
    }
    extraRegionCursor = position->extraRegionIndices;
    extraRegionEnd = extraRegionCursor + position->extraRegionCount;
    while (extraRegionCursor < extraRegionEnd)
    {
        regions = actor->zones[*extraRegionCursor]->regions;
        referenceCursor = regions->referenceItems;
        referenceEnd = referenceCursor + regions->referenceCount;
        while (referenceCursor < referenceEnd)
        {
            reference = *referenceCursor;
            if (EQ_CDECL(int, 0x100190B6, (int, void *, int *))(reference, referenceList, &insertIndex) == 0 &&
                    EQ_CDECL(int, 0x100190F9, (int, void *, int))(reference, referenceList, insertIndex) != 0)
            {
                goto insertion_failed;
            }
            referenceCursor++;
        }
        extraRegionCursor++;
    }
    goto done;
insertion_failed:
    result = 0xffff3d3e;
done:
    return result;
}

// FUNCTION: EQGFX 0x10020AFF
// Collect actors present in any region belonging to a zone.
// __cdecl. Clears referenceList, then adds the actor of each region entry not yet in it; returns 0, or 0xffff3d3e when an insert fails.
int __cdecl t3dGetActorsTouchingZone(T3dZoneActorList_guess *actorList, ZoneRegions_guess *zone, void *referenceList)
{
    int result;
    int insertIndex;
    int *regionEnd;
    int *regionCursor;
    t3dRegionSet_guess *regions;
    int *entryEnd;
    int *entryCursor;
    T3dZoneActorEntry_guess *actor;

    result = 0;
    EQ_CDECL(void, 0x1001905A, (void *))(referenceList);
    regionCursor = zone->regionList;
    regionEnd = regionCursor + zone->regionCount;
    while (regionCursor < regionEnd)
    {
        regions = ((t3dWorldObject_guess *)*regionCursor)->regions;
        entryCursor = regions->entries;
        entryEnd = entryCursor + regions->entryCount;
        for (; entryCursor < entryEnd; entryCursor++)
        {
            actor = actorList->actors[*entryCursor];
            if (EQ_CDECL(int, 0x100190B6, (void *, void *, int *))(actor, referenceList, &insertIndex) != 1 &&
                EQ_CDECL(int, 0x100190F9, (void *, void *, int))(actor, referenceList, insertIndex) != 0)
            {
                goto failed;
            }
        }
        regionCursor++;
    }
    goto done;
failed:
    result = 0xffff3d3e;
done:
    return result;
}

// FUNCTION: EQGFX 0x10020B94
// Send a message to actors touching a zone across its regions.
// __cdecl. Gets the actors touching the zone into actorList's referenceList; returns 0xffff3d3e when that fails, else sends message to each of them and returns 0.
int __cdecl t3dSendMessageToActorsTouchingZone(T3dZoneActorList_guess *actorList, ZoneRegions_guess *zone, void *message)
{
    int result;
    T3dZoneReferenceList_guess *referenceList;
    T3dZoneActorEntry_guess **actorCursor;
    T3dZoneActorEntry_guess **actorEnd;

    result = 0;
    referenceList = &actorList->referenceList;
    if (EQ_CDECL(int, 0x10020AFF, (T3dZoneActorList_guess *, ZoneRegions_guess *, void *))(
            actorList, zone, referenceList) != 0)
    {
        result = 0xffff3d3e;
    }
    else
    {
        actorCursor = referenceList->actors;
        actorEnd = actorCursor + referenceList->count;
        for (; actorCursor < actorEnd; actorCursor++)
        {
            EQ_CDECL(void, 0x1000A553, (T3dZoneActorList_guess *, T3dZoneActorEntry_guess *, void *))(
                actorList, *actorCursor, message);
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10020bea
// __cdecl. Sets each region's ambient light, returns 0xffff3d3e on first failure.
int __cdecl t3dSetZoneAmbientLight(int unused, ZoneRegions_guess *zone, int ambient)
{
    int *current;
    int *end;
    int status;

    current = zone->regionList;
    status = 0;
    end = current + zone->regionCount;
    while (current < end)
    {
        if (EQ_CDECL(int, 0x100194A2, (int, int))(*current, ambient) != 0)
        {
            status = 0xffff3d3e;
            break;
        }
        current++;
    }
    return status;
}

#define POINT_LIGHT_IN_ZONE 0x40

// FUNCTION: EQGFX 0x10020c22
// Attaches a point light (flag 0x40) to a zone's regions, appending it to each new region's light list.
int __cdecl t3dAddPointLightToZone(T3dZoneActorList_guess *zone, PointLight_guess *light, ZoneRegions_guess *region)
{
    int result;
    T3dZoneRegionIndex_guess **zoneRegionCursor;
    T3dZoneRegionIndex_guess **zoneRegionEnd;
    unsigned int *scratchWrite;
    unsigned int *lightRegionCursor;
    unsigned int *lightRegionEnd;
    unsigned int *newRegionCursor;
    unsigned int *newRegionEnd;
    unsigned int *newRegionWrite;
    unsigned int *regionIndexCursor;
    unsigned int *regionIndexEnd;
    unsigned int *lightRegionStart;
    unsigned int *lightRegionTail;
    unsigned int *newRegionStart;
    unsigned int *newRegionTail;
    unsigned int *mergedRegionWrite;
    T3dZoneActorEntry_guess *regionActor;
    T3dZonePointLightArray_guess *regionLightArray;
    unsigned int newRegionCount;
    unsigned int regionIndex;
    unsigned int combinedRegionCount;
    unsigned int newCapacity;
    int allocationResult;

    result = 0;
    if ((light->flags & POINT_LIGHT_IN_ZONE) == 0)
        goto failure;

    zoneRegionCursor = (T3dZoneRegionIndex_guess **)region->regionList;
    zoneRegionEnd = zoneRegionCursor + region->regionCount;
    scratchWrite = zone->pointLightRegionIndices;
    for (; zoneRegionCursor < zoneRegionEnd; zoneRegionCursor++)
    {
        *scratchWrite = (*zoneRegionCursor)->regionIndex;
        scratchWrite++;
    }

    lightRegionCursor = (unsigned int *)light->entries;
    lightRegionEnd = lightRegionCursor + light->entryCount;
    newRegionCursor = zone->pointLightRegionIndices;
    newRegionEnd = newRegionCursor + region->regionCount;
    newRegionWrite = newRegionCursor;
    newRegionCount = 0;
    while (lightRegionCursor < lightRegionEnd)
    {
        if (newRegionEnd <= newRegionCursor)
            goto mergeComplete;
        if (*newRegionCursor < *lightRegionCursor)
        {
            regionIndex = *newRegionCursor;
            newRegionCursor++;
            newRegionCount++;
            *newRegionWrite = regionIndex;
            newRegionWrite++;
        }
        else if (*lightRegionCursor < *newRegionCursor)
        {
            lightRegionCursor++;
        }
        else
        {
            newRegionCursor++;
        }
    }
    if (newRegionCursor < newRegionEnd)
    {
        newRegionCount += (unsigned int)((int)newRegionEnd + (3 - (int)newRegionCursor)) >> 2;
        do
        {
            regionIndex = *newRegionCursor;
            newRegionCursor++;
            *newRegionWrite = regionIndex;
            newRegionWrite++;
        } while (newRegionCursor < newRegionEnd);
    }

mergeComplete:
    combinedRegionCount = light->entryCount + newRegionCount;
    if (light->entryCapacity <= combinedRegionCount)
    {
        allocationResult = EQ_CDECL(int, 0x10050C8C, (void *, int))(
            light->entries, (int)(combinedRegionCount * 4));
        if (allocationResult == 0)
            goto failure;
        light->entries = (int *)allocationResult;
        light->entryCapacity = combinedRegionCount;
    }

    regionIndexCursor = zone->pointLightRegionIndices;
    regionIndexEnd = regionIndexCursor + newRegionCount;
    for (; regionIndexCursor < regionIndexEnd; regionIndexCursor++)
    {
        regionActor = (T3dZoneActorEntry_guess *)zone->regionReferences[*regionIndexCursor];
        regionLightArray = regionActor->pointLightArray;
        if (regionLightArray->capacity <= regionLightArray->count)
        {
            newCapacity = (regionLightArray->capacity >> 2) + 1 + regionLightArray->capacity;
            allocationResult = EQ_CDECL(int, 0x10050C8C, (void *, int))(
                regionLightArray->lights, (int)(newCapacity * 4));
            if (allocationResult == 0)
                goto failure;
            regionLightArray->lights = (void **)allocationResult;
            regionLightArray->capacity = newCapacity;
        }
    }

    lightRegionStart = (unsigned int *)light->entries;
    lightRegionTail = lightRegionStart + light->entryCount - 1;
    newRegionStart = zone->pointLightRegionIndices;
    newRegionTail = newRegionStart + newRegionCount - 1;
    mergedRegionWrite = lightRegionTail + newRegionCount;
    if (newRegionStart <= newRegionTail)
    {
        do
        {
            if (lightRegionTail < lightRegionStart)
                break;
            if (*lightRegionTail < *newRegionTail)
            {
                *mergedRegionWrite = *newRegionTail;
                newRegionTail--;
            }
            else
            {
                *mergedRegionWrite = *lightRegionTail;
                lightRegionTail--;
            }
            mergedRegionWrite--;
        } while (newRegionStart <= newRegionTail);
        for (; newRegionStart <= newRegionTail; newRegionTail--)
        {
            *mergedRegionWrite = *newRegionTail;
            mergedRegionWrite--;
        }
    }

    light->entryCount = light->entryCount + newRegionCount;
    regionIndexCursor = zone->pointLightRegionIndices;
    regionIndexEnd = regionIndexCursor + newRegionCount;
    for (; regionIndexCursor < regionIndexEnd; regionIndexCursor++)
    {
        regionActor = (T3dZoneActorEntry_guess *)zone->regionReferences[*regionIndexCursor];
        regionLightArray = regionActor->pointLightArray;
        regionLightArray->lights[regionLightArray->count] = light;
        regionLightArray->count++;
    }
    goto done;

failure:
    result = 0xffff3d3e;
done:
    return result;
}

// FUNCTION: EQGFX 0x10020E13
int __cdecl t3dRemovePointLightFromZone(T3dZoneActorList_guess *zone, PointLight_guess *light, ZoneRegions_guess *region)
{
    int result;
    int *lightEntry;
    int *lightEntryEnd;
    unsigned int *referenceCursor;
    unsigned int *referenceEnd;
    unsigned int *referenceWrite;
    unsigned int *regionCursor;
    unsigned int *regionEnd;
    int *lightRead;
    int *lightWrite;
    int retainedCount;
    unsigned int referenceValue;
    unsigned int remainingCount;
    t3dWorldObject_guess **actorCursor;
    t3dWorldObject_guess **actorEnd;
    t3dWorldObject_guess *actor;
    t3dRegionSet_guess *actorRegions;
    int **objectCursor;
    int **objectEnd;

    result = 0;
    if ((light->flags & POINT_LIGHT_IN_ZONE) == 0)
    {
        result = 0xffff3d3e;
        goto done;
    }
    EQ_CDECL(void, 0x1001905A, (T3dZoneReferenceList_guess *))(&zone->referenceList);
    lightEntry = light->entries;
    lightEntryEnd = lightEntry + light->entryCount;
    for (; lightEntry < lightEntryEnd; lightEntry++)
    {
        if (EQ_CDECL(int, 0x10019065, (void *, T3dZoneReferenceList_guess *))(
                zone->regionReferences[*lightEntry], &zone->referenceList) < 0)
        {
            result = 0xffff3d3e;
            goto done;
        }
    }

    referenceCursor = (unsigned int *)zone->referenceList.actors;
    referenceEnd = referenceCursor + zone->referenceList.count;
    light->entryCount = 0;
    retainedCount = 0;
    regionCursor = (unsigned int *)region->regionList;
    regionEnd = regionCursor + region->regionCount;
    referenceWrite = referenceCursor;
    lightRead = light->entries;
    lightWrite = light->entries;
    while (regionCursor < regionEnd)
    {
        if (referenceEnd <= referenceCursor)
            goto compacted;
        referenceValue = *referenceCursor;
        if (*regionCursor == referenceValue)
        {
            *referenceWrite = referenceValue;
            referenceWrite++;
            retainedCount++;
            referenceCursor++;
            regionCursor++;
            lightRead++;
        }
        else if (*regionCursor < referenceValue)
        {
            regionCursor++;
        }
        else
        {
            referenceCursor++;
            *lightWrite = *lightRead;
            light->entryCount++;
            lightRead++;
            lightWrite++;
        }
    }
    if (referenceCursor < referenceEnd)
    {
        remainingCount = ((unsigned int)referenceEnd - (unsigned int)referenceCursor + 3) >> 2;
        do
        {
            *lightWrite = *lightRead;
            lightWrite++;
            light->entryCount++;
            remainingCount--;
        } while (remainingCount != 0);
    }
compacted:
    zone->referenceList.count = retainedCount;
    actorCursor = (t3dWorldObject_guess **)zone->referenceList.actors;
    actorEnd = actorCursor + retainedCount;
    do
    {
        if (actorEnd <= actorCursor)
            goto done;
        actor = *actorCursor;
        actorRegions = actor->regions;
        objectCursor = actorRegions->objects;
        objectEnd = objectCursor + actorRegions->objectCount;
        for (; objectCursor < objectEnd; objectCursor++)
        {
            if (*objectCursor == (int *)light)
            {
                if (objectCursor != objectEnd - 1)
                {
                    EQ_CDECL(void *, 0x100679F0, (void *, const void *, unsigned int))(
                        objectCursor,
                        objectCursor + 1,
                        (unsigned int)(((int)(objectEnd - 1) - (int)objectCursor >> 2) << 2));
                }
                actorRegions->objectCount--;
                break;
            }
        }
        actorCursor++;
    } while (1);
done:
    return result;
}

// FUNCTION: EQGFX 0x10020F73
// __cdecl. Tests the light's position (+0x14) against world and region; returns the callee's result unchanged.
int __cdecl t3dIsPointLightPositionContainedInZone(int world, PointLight_guess *light, int region)
{
    return EQ_CDECL(int, 0x1002081D, (int, int, int))(world, (int)light->position, region);
}

// FUNCTION: EQGFX 0x10020F8C
// Find zones containing the region at a point light's position.
// __cdecl. Calls the zone lookup with the light's position (+0x14); returns 0xFFFF3D3E when the callee returns nonzero, else 0.
int __cdecl t3dGetZonesContainingPointLightPosition(int world, PointLight_guess *light, int region)
{
    int result;
    int lookupStatus;

    result = 0;
    lookupStatus = EQ_CDECL(int, 0x1002077A, (int, int, int))(world, (int)light->position, region);
    if (lookupStatus != 0)
    {
        result = 0xffff3d3e;
    }
    return result;
}

#define T3D_POINT_LIGHT_REFERENCE_ERROR 0xffff3d3e

// FUNCTION: EQGFX 0x10020FB4
// Find point lights whose positions fall inside a zone.
int __cdecl t3dGetPointLightsPositionedInZone(t3dWorld_guess *world, ZoneRegions_guess *zone, T3dZoneReferenceList_guess *referenceList)
{
    struct PointLightZoneScanState
    {
        int referenceIndex;
        int result;
    } scanState;
    PointLight_guess **pointLightEntry;
    PointLight_guess **pointLightEnd;
    PointLight_guess *pointLight;
    int region;

    scanState.result = 0;
    EQ_CDECL(void, 0x1001905A, (T3dZoneReferenceList_guess *))(referenceList);
    pointLightEntry = world->pointLightEntries;
    pointLightEnd = pointLightEntry + world->pointLightEntryCount;
    while (1)
    {
        if (pointLightEnd <= pointLightEntry)
        {
            return scanState.result;
        }
        pointLight = *pointLightEntry;
        if ((unsigned int)pointLight > world->pointLightEntryCount &&
            (region = EQ_CDECL(int, 0x10019324, (int, int))((int)world, (int)pointLight->position)) != 0 &&
            EQ_CDECL(int, 0x100190B6, (int, void *, int *))(region, &zone->referenceList, &scanState.referenceIndex) == 1 &&
            EQ_CDECL(int, 0x10019065, (int, T3dZoneReferenceList_guess *))((int)pointLight, referenceList) != 0)
        {
            scanState.result = T3D_POINT_LIGHT_REFERENCE_ERROR;
            break;
        }
        pointLightEntry++;
    }
    return scanState.result;
}

// FUNCTION: EQGFX 0x10021042
// Collect point lights linked to the regions in a light's entries.
// __cdecl. Clears referenceList, then adds each object tagged 0x1e in the region sets of the light's entries that is not listed yet (world is unused). Returns 0xFFFF3D3E when an insert fails and stops there, else 0.
int __cdecl t3dGetPointLightsInfluencingZone(int world, t3dLight_guess *light, T3dZoneReferenceList_guess *referenceList)
{
    int result;
    int insertIndex;
    t3dWorldObject_guess **entry;
    t3dWorldObject_guess **entryEnd;
    t3dRegionSet_guess *regions;
    int **objectSlot;
    int **objectEnd;
    int *object;

    result = 0;
    EQ_CDECL(void, 0x1001905A, (T3dZoneReferenceList_guess *))(referenceList);
    entry = light->entries;
    entryEnd = entry + light->entryCount;
    while (entry < entryEnd)
    {
        regions = (*entry)->regions;
        objectSlot = regions->objects;
        objectEnd = objectSlot + regions->objectCount;
        for (; objectSlot < objectEnd; objectSlot++)
        {
            object = *objectSlot;
            if (*object == 0x1e &&
                EQ_CDECL(int, 0x100190B6, (int *, T3dZoneReferenceList_guess *, int *))(object, referenceList, &insertIndex) == 0 &&
                EQ_CDECL(int, 0x100190F9, (int *, T3dZoneReferenceList_guess *, int))(object, referenceList, insertIndex) != 0)
            {
                result = 0xffff3d3e;
                goto done;
            }
        }
        entry++;
    }
done:
    return result;
}

// FUNCTION: EQGFX 0x100210CF
// Attaches a directional light to a zone's regions, inserting it first in each new region's light list.
// __cdecl. Adds the region indices of the region list that the light does not list yet to its sorted entries, grows the full point light arrays of those regions and puts the light first in each. Returns 0xFFFF3D3E when an allocation fails, else 0.
int __cdecl t3dAddDirectionalLightToZone(T3dZoneActorList_guess *zone, DirectionalLight_guess *light, ZoneRegions_guess *region)
{
    int result;
    unsigned int newRegionCount;
    T3dZoneRegionIndex_guess **zoneRegionCursor;
    T3dZoneRegionIndex_guess **zoneRegionEnd;
    unsigned int *scratchWrite;
    unsigned int *lightRegionStart;
    unsigned int *lightRegionCursor;
    unsigned int *lightRegionEnd;
    unsigned int *lightRegionTail;
    unsigned int *newRegionCursor;
    unsigned int *newRegionEnd;
    unsigned int *newRegionStart;
    unsigned int *newRegionTail;
    unsigned int *newRegionWrite;
    unsigned int *mergedRegionWrite;
    unsigned int *regionIndexCursor;
    unsigned int *regionIndexEnd;
    T3dZoneActorEntry_guess *regionActor;
    T3dZonePointLightArray_guess *regionLightArray;
    unsigned int regionIndex;
    unsigned int combinedRegionCount;
    unsigned int newCapacity;
    int allocationResult;

    result = 0;
    zoneRegionCursor = (T3dZoneRegionIndex_guess **)region->regionList;
    zoneRegionEnd = zoneRegionCursor + region->regionCount;
    scratchWrite = zone->pointLightRegionIndices;
    for (; zoneRegionCursor < zoneRegionEnd; zoneRegionCursor++)
    {
        *scratchWrite = (*zoneRegionCursor)->regionIndex;
        scratchWrite++;
    }

    lightRegionCursor = (unsigned int *)light->entries;
    lightRegionEnd = lightRegionCursor + light->entryCount;
    newRegionCursor = zone->pointLightRegionIndices;
    newRegionEnd = newRegionCursor + region->regionCount;
    newRegionWrite = newRegionCursor;
    newRegionCount = 0;
    while (lightRegionCursor < lightRegionEnd)
    {
        if (newRegionEnd <= newRegionCursor)
            goto mergeComplete;
        if (*newRegionCursor < *lightRegionCursor)
        {
            regionIndex = *newRegionCursor;
            newRegionCursor++;
            newRegionCount++;
            *newRegionWrite = regionIndex;
            newRegionWrite++;
        }
        else if (*lightRegionCursor < *newRegionCursor)
        {
            lightRegionCursor++;
        }
        else
        {
            newRegionCursor++;
        }
    }
    if (newRegionCursor < newRegionEnd)
    {
        newRegionCount += (unsigned int)((int)newRegionEnd + (3 - (int)newRegionCursor)) >> 2;
        do
        {
            regionIndex = *newRegionCursor;
            newRegionCursor++;
            *newRegionWrite = regionIndex;
            newRegionWrite++;
        } while (newRegionCursor < newRegionEnd);
    }

mergeComplete:
    combinedRegionCount = light->entryCount + newRegionCount;
    if (light->entryCapacity <= combinedRegionCount)
    {
        allocationResult = EQ_CDECL(int, 0x10050C8C, (void *, int))(
            light->entries, (int)(combinedRegionCount * 4));
        if (allocationResult == 0)
            goto failure;
        light->entries = (int *)allocationResult;
        light->entryCapacity = combinedRegionCount;
    }

    regionIndexCursor = zone->pointLightRegionIndices;
    regionIndexEnd = regionIndexCursor + newRegionCount;
    for (; regionIndexCursor < regionIndexEnd; regionIndexCursor++)
    {
        regionActor = (T3dZoneActorEntry_guess *)zone->regionReferences[*regionIndexCursor];
        regionLightArray = regionActor->pointLightArray;
        if (regionLightArray->capacity <= regionLightArray->count)
        {
            newCapacity = (regionLightArray->capacity >> 2) + 1 + regionLightArray->capacity;
            allocationResult = EQ_CDECL(int, 0x10050C8C, (void *, int))(
                regionLightArray->lights, (int)(newCapacity * 4));
            if (allocationResult == 0)
                goto failure;
            regionLightArray->lights = (void **)allocationResult;
            regionLightArray->capacity = newCapacity;
        }
    }

    lightRegionStart = (unsigned int *)light->entries;
    lightRegionTail = lightRegionStart + light->entryCount - 1;
    newRegionStart = zone->pointLightRegionIndices;
    newRegionTail = newRegionStart + newRegionCount - 1;
    mergedRegionWrite = lightRegionTail + newRegionCount;
    if (newRegionStart <= newRegionTail)
    {
        do
        {
            if (lightRegionTail < lightRegionStart)
                break;
            if (*lightRegionTail < *newRegionTail)
            {
                *mergedRegionWrite = *newRegionTail;
                newRegionTail--;
            }
            else
            {
                *mergedRegionWrite = *lightRegionTail;
                lightRegionTail--;
            }
            mergedRegionWrite--;
        } while (newRegionStart <= newRegionTail);
        for (; newRegionStart <= newRegionTail; newRegionTail--)
        {
            *mergedRegionWrite = *newRegionTail;
            mergedRegionWrite--;
        }
    }

    light->entryCount = light->entryCount + newRegionCount;
    regionIndexCursor = zone->pointLightRegionIndices;
    regionIndexEnd = regionIndexCursor + newRegionCount;
    for (; regionIndexCursor < regionIndexEnd; regionIndexCursor++)
    {
        regionActor = (T3dZoneActorEntry_guess *)zone->regionReferences[*regionIndexCursor];
        regionLightArray = regionActor->pointLightArray;
        if (regionLightArray->count != 0)
        {
            EQ_CDECL(void *, 0x100679F0, (void *, const void *, unsigned int))(
                regionLightArray->lights + 1, regionLightArray->lights, regionLightArray->count * 4);
        }
        regionLightArray->lights[0] = light;
        regionLightArray->count++;
    }
    goto done;

failure:
    result = 0xffff3d3e;
done:
    return result;
}

// FUNCTION: EQGFX 0x100212E0
int __cdecl t3dRemoveDirectionalLightFromZone(T3dZoneActorList_guess *zone, DirectionalLight_guess *light, ZoneRegions_guess *region)
{
    int result;
    int *lightEntry;
    int *lightEntryEnd;
    unsigned int *referenceCursor;
    unsigned int *referenceEnd;
    unsigned int *referenceWrite;
    unsigned int *regionCursor;
    unsigned int *regionEnd;
    int *lightRead;
    int *lightWrite;
    int retainedCount;
    unsigned int referenceValue;
    t3dWorldObject_guess **actorCursor;
    t3dWorldObject_guess **actorEnd;
    t3dWorldObject_guess *actor;
    t3dRegionSet_guess *actorRegions;
    int **objectCursor;
    int **objectEnd;

    result = 0;
    EQ_CDECL(void, 0x1001905A, (T3dZoneReferenceList_guess *))(&zone->referenceList);
    lightEntry = light->entries;
    lightEntryEnd = lightEntry + light->entryCount;
    for (; lightEntry < lightEntryEnd; lightEntry++)
    {
        if (EQ_CDECL(int, 0x10019065, (void *, T3dZoneReferenceList_guess *))(
                zone->regionReferences[*lightEntry], &zone->referenceList) < 0)
        {
            result = 0xffff3d3e;
            goto done;
        }
    }

    referenceCursor = (unsigned int *)zone->referenceList.actors;
    referenceEnd = referenceCursor + zone->referenceList.count;
    light->entryCount = 0;
    retainedCount = 0;
    regionCursor = (unsigned int *)region->regionList;
    regionEnd = regionCursor + region->regionCount;
    referenceWrite = referenceCursor;
    lightRead = light->entries;
    lightWrite = light->entries;
    while (regionCursor < regionEnd)
    {
        if (referenceEnd <= referenceCursor)
            goto compacted;
        referenceValue = *referenceCursor;
        if (*regionCursor == referenceValue)
        {
            *referenceWrite = referenceValue;
            referenceWrite++;
            retainedCount++;
            referenceCursor++;
            regionCursor++;
            lightRead++;
        }
        else if (*regionCursor < referenceValue)
        {
            regionCursor++;
        }
        else
        {
            referenceCursor++;
            *lightWrite = *lightRead;
            light->entryCount++;
            lightRead++;
            lightWrite++;
        }
    }
    if (referenceCursor < referenceEnd)
    {
        unsigned int remainingCount;

        remainingCount = ((unsigned int)referenceEnd - (unsigned int)referenceCursor + 3) >> 2;
        do
        {
            *lightWrite = *lightRead;
            lightWrite++;
            light->entryCount++;
            remainingCount--;
        } while (remainingCount != 0);
    }
compacted:
    zone->referenceList.count = retainedCount;
    actorCursor = (t3dWorldObject_guess **)zone->referenceList.actors;
    actorEnd = actorCursor + retainedCount;
    do
    {
        if (actorEnd <= actorCursor)
            goto done;
        actor = *actorCursor;
        actorRegions = actor->regions;
        objectCursor = actorRegions->objects;
        objectEnd = objectCursor + actorRegions->objectCount;
        for (; objectCursor < objectEnd; objectCursor++)
        {
            if (*objectCursor == (int *)light)
            {
                if (objectCursor != objectEnd - 1)
                {
                    EQ_CDECL(void *, 0x100679F0, (void *, const void *, unsigned int))(
                        objectCursor,
                        objectCursor + 1,
                        (unsigned int)(((int)(objectEnd - 1) - (int)objectCursor >> 2) << 2));
                }
                actorRegions->objectCount--;
                break;
            }
        }
        actorCursor++;
    } while (1);
done:
    return result;
}

// FUNCTION: EQGFX 0x10021436
// Check whether a directional light affects a region.
// __cdecl. Returns 1 when the callee finds one of the light's entry objects in the region's reference list (stops at the first hit), else 0.
int __cdecl t3dIsDirectionalLightInfluencingZone(t3dWorld_guess *world, DirectionalLight_guess *light, ZoneRegions_guess *region)
{
    int found;
    int *entry;
    int *entryEnd;
    int referenceList;

    found = 0;
    entry = light->entries;
    entryEnd = entry + light->entryCount;
    referenceList = (int)&region->referenceList;
    while (entry < entryEnd)
    {
        if (EQ_CDECL(int, 0x100190B6, (int, int, DirectionalLight_guess **))(world->objectTable[*entry],
                referenceList, &light) == 1)
        {
            found = 1;
            break;
        }
        entry++;
    }
    return found;
}

// FUNCTION: EQGFX 0x10021487
// Collect directional lights linked to the regions in a light's entries.
// __cdecl. Clears the reference list, then adds each tag-0x1f item of the region sets of the light's entries that is not yet in the list; returns 0xFFFF3D3E when an insert fails, else 0.
int __cdecl t3dGetDirectionalLightsInfluencingZone(t3dWorld_guess *world, t3dLight_guess *light, void *referenceList)
{
    int result;
    int insertPosition;
    t3dWorldObject_guess **entry;
    t3dWorldObject_guess **entryEnd;
    t3dRegionSet_guess *regions;
    int **reference;
    int *referenceItem;

    result = 0;
    EQ_CDECL(void, 0x1001905A, (void *))(referenceList);
    entry = light->entries;
    entryEnd = entry + light->entryCount;
    while (entry < entryEnd)
    {
        regions = (*entry)->regions;
        reference = regions->objects;
        light = (t3dLight_guess *)(reference + regions->objectCount);
        while (reference < (int **)light)
        {
            referenceItem = *reference;
            if (*referenceItem == 0x1f &&
                EQ_CDECL(int, 0x100190B6, (int *, void *, int *))(referenceItem, referenceList, &insertPosition) == 0 &&
                EQ_CDECL(int, 0x100190F9, (int *, void *, int))(referenceItem, referenceList, insertPosition) != 0)
            {
                goto insertFailed;
            }
            reference++;
        }
        entry++;
    }
    goto done;
insertFailed:
    result = 0xffff3d3e;
done:
    return result;
}

// FUNCTION: EQGFX 0x10021514
// Find zones linked to the regions affected by a directional light.
// __cdecl. Clears the reference list, then adds each reference item of the zones named by the light's entries that is not yet in the list; returns 0xFFFF3D3E when an insert fails, else 0.
int __cdecl t3dGetZonesInfluencedByDirectionalLight(t3dWorld_guess *world, DirectionalLight_guess *light, void *referenceList)
{
    int result;
    int insertPosition;
    int *entry;
    int *entryEnd;
    t3dRegionSet_guess *regions;
    int *reference;
    int *referenceEnd;
    int value;

    result = 0;
    EQ_CDECL(void, 0x1001905A, (void *))(referenceList);
    entry = light->entries;
    entryEnd = entry + light->entryCount;
    while (entry < entryEnd)
    {
        regions = ((t3dWorldObject_guess *)world->objectTable[*entry])->regions;
        reference = regions->referenceItems;
        referenceEnd = reference + regions->referenceCount;
        for (; reference < referenceEnd; reference++)
        {
            value = *reference;
            if (EQ_CDECL(int, 0x100190B6, (int, void *, int *))(value, referenceList, &insertPosition) == 0)
            {
                if (EQ_CDECL(int, 0x100190F9, (int, void *, int))(value, referenceList, insertPosition) != 0)
                {
                    result = 0xffff3d3e;
                    goto done;
                }
            }
        }
        entry++;
    }
done:
    return result;
}

#define ZONE_NAME_PREFIX_80000000 EQ_ARRAY(char, 0x10086FF4)
#define ZONE_NAME_PREFIX_20000000 EQ_ARRAY(char, 0x10086FF0)
#define ZONE_NAME_PREFIX_40000000 EQ_ARRAY(char, 0x10086FEC)

// FUNCTION: EQGFX 0x100215A4
// Applies zone-name-based flags to member regions; the specific environmental effects are unclear.
// __cdecl (callee pops 0 bytes), int (callers read EAX; always 0). For each zone pointer above zoneCount, the zone's two name strings
// pick environment flags (2-char prefix: 0x80000000, 0x20000000 or 0x40000000), ORed into the dword at +0xc of every region in its list.
int __cdecl t3dTranslateZonesToRegionEnvironmentFlags(t3dWorld_guess *world)
{
    unsigned int zoneIndex;
    unsigned int zoneCount;
    ZoneRegions_guess *zone;
    char *extraName;
    char *keyName;
    unsigned int environmentFlags;
    unsigned int regionIndex;
    ZoneRegions_guess *region;

    zoneIndex = 0;
    zoneCount = world->zoneCount;
    if (zoneCount > 0)
    {
        do
        {
            zone = world->zones[zoneIndex];
            if ((unsigned int)zone > zoneCount)
            {
                extraName = (char *)zone->extraBlock;
                keyName = (char *)zone->dictionaryKey;
                environmentFlags = 0;
                if (extraName != 0)
                {
                    if (EQ_CDECL(int, 0x100689E0, (char *, char *, unsigned int))(extraName, ZONE_NAME_PREFIX_80000000, 2) == 0)
                    {
                        environmentFlags = 0x80000000;
                    }
                    else if (EQ_CDECL(int, 0x100689E0, (char *, char *, unsigned int))(extraName, ZONE_NAME_PREFIX_20000000, 2) == 0)
                    {
                        environmentFlags = 0x20000000;
                    }
                    else if (EQ_CDECL(int, 0x100689E0, (char *, char *, unsigned int))(extraName, ZONE_NAME_PREFIX_40000000, 2) == 0)
                    {
                        environmentFlags = 0x40000000;
                    }
                }
                if (keyName != 0)
                {
                    if (EQ_CDECL(int, 0x100689E0, (char *, char *, unsigned int))(keyName, ZONE_NAME_PREFIX_80000000, 2) == 0)
                    {
                        environmentFlags = 0x80000000;
                    }
                    else if (EQ_CDECL(int, 0x100689E0, (char *, char *, unsigned int))(keyName, ZONE_NAME_PREFIX_20000000, 2) == 0)
                    {
                        environmentFlags = 0x20000000;
                    }
                    else if (EQ_CDECL(int, 0x100689E0, (char *, char *, unsigned int))(keyName, ZONE_NAME_PREFIX_40000000, 2) == 0)
                    {
                        environmentFlags = 0x40000000;
                    }
                }
                if (environmentFlags != 0)
                {
                    for (regionIndex = 0; regionIndex < zone->regionCount; regionIndex++)
                    {
                        region = (ZoneRegions_guess *)zone->regionList[regionIndex];
                        *(unsigned int *)&region->flags |= environmentFlags;
                    }
                }
            }
            zoneIndex++;
            zoneCount = world->zoneCount;
        } while (zoneIndex < zoneCount);
    }
    return 0;
}

#define FAST_FILE_SYSTEM_DATA_BYTES 8
#define FAST_FILE_INIT_PATH_TEMPORARY(state) ((FilePath_guess *)((char *)&(state) - 0x14))

// FUNCTION: EQGFX 0x100216BD
// Initializes a path-associated resource context; the underlying loading operation is unclear.
void *__cdecl t3dFastFileInit(void *pathName)
{
    FastFileInitLocals_guess locals;
    char loadSucceeded;
    void *packFileSystemData;
    FastFileRefCounted_guess *reference;

    EQ_FASTCALL(void, 0x10051F30, (void *))(&locals.packFileSystemState);
    packFileSystemData = 0;
    EQ_THISCALL(void, 0x10021CC5, (FilePath_guess *, int, void *))(
        FAST_FILE_INIT_PATH_TEMPORARY(locals.packFileSystemState), 0, pathName);
    loadSucceeded = EQ_THISCALL(char, 0x10051F60, (void *, int, FilePath_guess))(
        &locals.packFileSystemState, 0,
        *FAST_FILE_INIT_PATH_TEMPORARY(locals.packFileSystemState));
    if (loadSucceeded != 0)
    {
        packFileSystemData = EQ_CDECL(void *, 0x10068C72, (unsigned int))(FAST_FILE_SYSTEM_DATA_BYTES);
        if (packFileSystemData == 0)
        {
            packFileSystemData = 0;
        }
        else
        {
            EQ_THISCALL(void, 0x10021938, (void *, int, int))(packFileSystemData, 0, 0);
        }
        if (packFileSystemData == 0)
        {
            EQ_THISCALL(void, 0x10021CC5, (FilePath_guess *, int, void *))(
                FAST_FILE_INIT_PATH_TEMPORARY(locals.packFileSystemState), 0, pathName);
            EQ_THISCALL(void, 0x10052500, (void *, int, FilePath_guess))(
                &locals.packFileSystemState, 0,
                *FAST_FILE_INIT_PATH_TEMPORARY(locals.packFileSystemState));
        }
        else
        {
            EQ_THISCALL(void, 0x10021CC5, (FilePath_guess *, int, void *))(
                &locals.resultPath, 0, pathName);
            EQ_THISCALL(void, 0x10021C94, (void *, int, FilePath_guess *))(
                packFileSystemData, 0, &locals.resultPath);
            reference = (FastFileRefCounted_guess *)locals.resultPath.words[0];
            if (reference != 0)
            {
                EQ_FASTCALL(void, 0x1002190D, (FastFileRefCounted_guess *))(reference);
            }
        }
    }
    return packFileSystemData;
}

// FUNCTION: EQGFX 0x10021748
// Release a fast-file reference and free its wrapper.
// __cdecl. Calls two methods with this = &ref (the second takes a counted copy of the handle by value), releases the object, frees the handle.
void __cdecl t3dFastFileFinish(FastFileRef_guess *ref)
{
    FastFileRef_guess copy;

    if (ref != 0)
    {
        EQ_FASTCALL(void, 0x10051F30, (FastFileRef_guess **))(&ref);
        copy.object = ref->object;
        if (copy.object != 0)
        {
            copy.object->referenceCount++;
        }
        copy.extra = ref->extra;
        EQ_FASTCALL(void, 0x10052500, (FastFileRef_guess **, FastFileRef_guess))(&ref, copy);
        if (ref->object != 0)
        {
            EQ_FASTCALL(void, 0x1002190D, (FastFileRefCounted_guess *))(ref->object);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(ref);
    }
}

// FUNCTION: EQGFX 0x10021791
// Prepares a resource handle from a context and path; the underlying open operation is unclear.
// @early-stop: the 0x20 local frame and explicit C call wrappers still differ from the original's stack temporaries.
/* fake match: test the allocation in EAX before copying the successful handle into the return local. */
int *__cdecl t3dFastFileOpen(void *fileReference, void *filePathName)
{
    FastFileRefCounted_guess **referenceHandle;
    FastFileRefCounted_guess **allocatedHandle;
    void **objectStorage;
    FastFileOpenTemporaries_guess temporaries;
    void *openTarget;

    referenceHandle = 0;
    if ((fileReference != 0) && (filePathName != 0))
    {
        if ((allocatedHandle = (FastFileRefCounted_guess **)EQ_CDECL(void *, 0x10068C72, (unsigned int))(4)) != 0)
        {
            *allocatedHandle = 0;
            referenceHandle = allocatedHandle;
        }
        else
        {
            referenceHandle = 0;
        }
        if (referenceHandle != 0)
        {
            objectStorage = (void **)EQ_CDECL(void *, 0x10068C72, (unsigned int))(4);
            if (objectStorage != 0)
            {
                *objectStorage = 0;
            }
            else
            {
                objectStorage = 0;
            }
            EQ_THISCALL(void, 0x10021B49, (FastFileRefCounted_guess **, int, void *))(&temporaries.pathAndTemporary.referenceStorage.temporaryObject, 0, objectStorage);
            EQ_THISCALL(void, 0x10021C32, (FastFileRefCounted_guess **, int, FastFileRefCounted_guess *))(
                referenceHandle, 0, temporaries.pathAndTemporary.referenceStorage.temporaryObject);
            if (*referenceHandle != 0)
            {
                EQ_THISCALL(void, 0x10021CC5, (FilePath_guess *, int, void *))(&temporaries.pathAndTemporary.filePath, 0, filePathName);
                EQ_THISCALL(void, 0x10021D3B, (FfSharedRef_guess *, int, void *))(&temporaries.sharedReference, 0, fileReference);
                openTarget = EQ_FASTCALL(void *, 0x10021C2C, (FastFileRefCounted_guess **))(referenceHandle);
                if (EQ_FASTCALL(char, 0x10052740, (void *, FfSharedRef_guess, FilePath_guess))(
                    openTarget, temporaries.sharedReference, temporaries.pathAndTemporary.filePath) == 0)
                {
                    if (*referenceHandle != 0)
                    {
                        EQ_FASTCALL(void, 0x1002190D, (FastFileRefCounted_guess *))(*referenceHandle);
                    }
                    EQ_CDECL(void, 0x10050CBB, (void *))(referenceHandle);
                    referenceHandle = 0;
                }
            }
            else
            {
                EQ_CDECL(void, 0x10050CBB, (void *))(referenceHandle);
                referenceHandle = 0;
            }
        }
    }
    return (int *)referenceHandle;
}

// FUNCTION: EQGFX 0x10021838
// __cdecl. Closes the file handle, releases the object, frees it; returns 1 if the handle was open.
int __cdecl t3dFastFileClose(FastFile_guess *fastFile)
{
    FastFileStream_guess *stream;
    FileHandle_guess *handle;
    int wasOpen;

    wasOpen = 0;
    if (fastFile != 0)
    {
        stream = fastFile->stream;
        handle = stream->handle;
        wasOpen = (handle->fileRecord != 0);
        EQ_FASTCALL(void, 0x10052A20, (FileHandle_guess *))(handle);
        EQ_FASTCALL(void, 0x10021C88, (FastFile_guess *))(fastFile);
        EQ_CDECL(void, 0x10050CBB, (void *))(fastFile);
    }
    return wasOpen;
}

// FUNCTION: EQGFX 0x1002186A
// Read bytes from a fast file into a buffer.
// __cdecl. Returns 0 for a null fastFile or buffer or a negative byteCount; else reads byteCount bytes (if any) and returns 1.
int __cdecl t3dFastFileRead(FastFile_guess *fastFile, void *buffer, int byteCount)
{
    int result;

    result = 0;
    if (fastFile != 0 && buffer != 0 && byteCount >= 0)
    {
        if (byteCount > 0)
        {
            EQ_THISCALL(void, 0x10052A90, (FileHandle_guess *, int, void *, int))(fastFile->stream->handle, 0, buffer, byteCount);
        }
        result = 1;
    }
    return result;
}

// FUNCTION: EQGFX 0x10021897
// Move the fast file's read position to an offset.
int __cdecl t3dFastFileSeek(FastFile_guess *fastFile, int offset, int origin)
{
    int result;
    int currentPosition;
    int fileSize;

    result = 0;
    if (fastFile != 0 && fastFile->stream->handle->fileRecord != 0 && offset >= 0)
    {
        currentPosition = EQ_THISCALL(int, 0x10052D00, (FileHandle_guess *))(fastFile->stream->handle);
        fileSize = EQ_THISCALL(int, 0x10052A80, (FileHandle_guess *))(fastFile->stream->handle);
        switch (origin)
        {
        case 0:
            if (offset >= fileSize)
                return 0;
            offset = offset - currentPosition;
            break;
        case 1:
            if (currentPosition + offset >= fileSize)
                return 0;
            break;
        case 2:
            if (offset >= fileSize)
                return 0;
            offset = (fileSize - currentPosition) - offset;
            break;
        default:
            return 0;
        }
        result = 1;
        EQ_THISCALL(void, 0x10052BC0, (FileHandle_guess *, int, int))(fastFile->stream->handle, 0, offset);
    }
    return result;
}
