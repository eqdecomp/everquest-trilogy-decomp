// unit_10007EF3.c - eqgfx_dx7.dll, the address run 0x10007EF3..0x10008CF2 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

#define T3D_GEOMETRY_RESOURCE_TYPE 0x0b
#define T3D_GEOMETRY_INSTANCE_TYPE 0x0c
#define T3D_GEOMETRY_REFERENCE_TYPE 0x37

typedef struct T3DGeometrySourceRecord_guess
{
    unsigned int reserved_00;
    int vertex_index_count;
    unsigned int *vertex_indices;
    void *vertex_positions;
    float normal_x;
    float normal_y;
    float normal_z;
    unsigned char reserved_1c[0x10];
} T3DGeometrySourceRecord_guess;

typedef struct T3DGeometryResource_guess
{
    unsigned int type;
    unsigned int reserved_04;
    char *name;
    unsigned int flags;
    int ref_count;
    unsigned int vertex_count;
    void *vertex_positions;
    float *vertex_normals;
    int group_count;
    T3DGeometrySourceRecord_guess *source_groups;
    float *source_user_data;
    void *instance;
} T3DGeometryResource_guess;

typedef struct T3DGeometryInstance_guess
{
    unsigned int type;
    unsigned int reserved_04;
    unsigned int reserved_08;
    unsigned int reserved_0c;
    int ref_count;
    unsigned int instance_value;
    void *output_records;
    unsigned int origin_x;
    unsigned int origin_y;
    unsigned int origin_z;
    unsigned int reserved_28;
    T3DGeometryResource_guess *resource;
} T3DGeometryInstance_guess;

typedef struct T3DGeometryResourceReference_guess
{
    unsigned int type;
    unsigned char reserved_04[0x18];
    unsigned int *shared_resource;
    unsigned char reserved_20[0x30];
} T3DGeometryResourceReference_guess;

typedef float (__cdecl *T3DGeometryLengthScaleFunction_guess)(float squared_length);
#define T3D_GEOMETRY_LENGTH_SCALE_FUNCTION EQ_GLOBAL(T3DGeometryLengthScaleFunction_guess, 0x100FC77C)

// The 2D sprite definition a sprite is built from (no include/ struct covers it): only the bounding radius
// slot is known, from 0x10007F3B.
typedef struct T3D2DSpriteDefinition_guess
{
    unsigned char unknown_00[0x2c]; // 0x00
    float bounding_radius;          // 0x2c: what t3dGet2DSpriteDefinitionBoundingRadius returns
} T3D2DSpriteDefinition_guess;

// 0x08 of the 2D sprite's flags dword at +0x0C: the animation is suspended (from t3dSuspend2DSpriteAnimation).
#define T3D2D_SPRITE_FLAG_SUSPENDED 0x08

// The 2D sprite object the t3dGet2DSprite* getters read (no include/ struct covers it): the suspend flag at +0x0C
// and the resume bits at +0x70, from 0x10007F43, the share count at +0x10, from 0x10008874, the skip-frames slot,
// from 0x10007F09, and the palette slot, from 0x10007F1F.
typedef struct T3D2DSprite_guess
{
    unsigned char unknown_00[0x0c]; // 0x00
    unsigned int flags;             // 0x0c: bit 0x08 suspends the animation (t3dSuspend2DSpriteAnimation)
    int ref_count;                  // 0x10: what t3dShare2DSprite increments (the share count)
    unsigned char unknown_14[0x1e]; // 0x14
    unsigned char pen;              // 0x32: what t3dGet2DSpritePen_guess reads and t3dSet2DSpritePen stores
    unsigned char unknown_33[0x3d]; // 0x33
    unsigned int anim_flags;        // 0x70: the bits a resume ORs in (t3dSuspend2DSpriteAnimation)
    int skip_frames;                // 0x74: what t3dGet2DSpriteSkipFrames returns
    unsigned char unknown_78[0x0c]; // 0x78
    unsigned int palette;           // 0x84: what t3dGet2DSpritePalette returns
} T3D2DSprite_guess;

// The three-float vector the center-offset, UV-origin, polygon-plane and plane getters copy as one 12-byte block (their
// MOVSD trio). Local stand-in: trilogy/utf8.h's EQVector3_guess is C++ (declares a member) and this file is C.
typedef struct EQGfxVector3_guess
{
    float x; // 0x0
    float y; // 0x4
    float z; // 0x8
} EQGfxVector3_guess;

// A 0x50-byte frame of a 3D sprite (no include/ struct covers it): the intensity float at +0x08, the ambient scaler
// float at +0x0C, the pen byte at +0x1A, the simple sprite at +0x1C and the buffer at +0x44 belong to it.
typedef struct T3D3DSpriteFrame_guess
{
    unsigned char unknown_00[0x04]; // 0x00
    int render_method;              // 0x04: what t3dGet3DSpritePolygonRenderMethod returns for this frame
    float intensity;                // 0x08: what t3dGet3DSpritePolygonConstantIntensity returns for this frame
    float ambient_scaler;           // 0x0c: what t3dGet3DSpritePolygonAmbientScaler returns for this frame
    unsigned char unknown_10[0x0a]; // 0x10
    unsigned char pen;              // 0x1a: what t3dGet3DSpritePolygonPen returns for this frame
    unsigned char unknown_1b[0x01]; // 0x1b
    int *simple_sprite;             // 0x1c: unshared and destroyed with the owning 3D sprite
    EQGfxVector3_guess uv_origin;   // 0x20: the 12 bytes t3dGet3DSpritePolygonUVOrigin copies out
    EQGfxVector3_guess u_axis;      // 0x2c: the 12 bytes t3dGet3DSpritePolygonUAxis copies out
    unsigned char unknown_38[0x0c]; // 0x38
    void *buffer;                   // 0x44: freed only when the 3D sprite owns the frame array
    unsigned char unknown_48[0x08]; // 0x48: pads the frame to its 0x50-byte stride
} T3D3DSpriteFrame_guess;

// The plane record t3dGet3DSpriteDefinitionPolygonPlane fills (no include/ struct covers it): the type dword at
// +0x00, the normal at +0x04 and the distance at +0x10.
typedef struct T3DPlane_guess
{
    int type;                  // 0x00: 0x2F, the plane type the getter writes
    EQGfxVector3_guess normal; // 0x04
    float distance;            // 0x10
} T3DPlane_guess;

// One 0x2C-byte polygon record of the 3D sprite definition's polygon array at +0x24 (no include/ struct covers
// it): the vertex count at +0x04, the vertex index list at +0x08 and the plane (normal at +0x10, distance at
// +0x1C) the polygon getters copy out.
typedef struct T3D3DSpritePolygon_guess
{
    unsigned char unknown_00[0x04]; // 0x00
    int vertex_count;               // 0x04: what t3dGet3DSpriteDefinitionPolygonNumVertices returns
    void *vertex_indices;           // 0x08: the vertex index list t3dGet3DSpriteDefinitionPolygonVertexIndexList copies out
    unsigned char unknown_0c[0x04]; // 0x0c
    EQGfxVector3_guess normal;      // 0x10: copied into the plane's normal
    float distance;                 // 0x1c: copied into the plane's distance
    unsigned char unknown_20[0x0c]; // 0x20: pads the record to its 0x2C-byte stride
} T3D3DSpritePolygon_guess;

// The shared 3D sprite definition a 3D sprite points at from +0x2C (no include/ struct covers it): the
// reference count, the frame count and the parent definition the frame array comes from.
typedef struct T3D3DSpriteDefinition_guess
{
    unsigned int type;              // 0x00: 0xB when the definition is valid (t3dIs3DSpriteDefinitionValid)
    unsigned char unknown_04[0x04]; // 0x04
    void *entry;                    // 0x08: dictionary entry, removed and freed when registered
    unsigned char flags;            // 0x0c: bit 0 = the entry is registered in the dictionary
    unsigned char unknown_0d[0x03]; // 0x0d
    int refcount;                   // 0x10: decremented when a 3D sprite is destroyed, 1 = last user
    void *volume;                   // 0x14: passed to t3dDestroyVolume
    T3D3DSpriteFrame_guess *frames; // 0x18: frame array, used when the sprite has none of its own
    unsigned char unknown_1c[0x04]; // 0x1c
    int frame_count;                // 0x20: number of 0x50-byte frames; bounds the polygon array at +0x24
    T3D3DSpritePolygon_guess *polygons; // 0x24: 0x2C-byte polygon records, one per frame
    unsigned char unknown_28[0x04]; // 0x28
    struct T3D3DSpriteDefinition_guess *parent; // 0x2c: definition the frame array is inherited from, type 0xC
} T3D3DSpriteDefinition_guess;

// The 3D sprite definition t3dGet3DSpriteDefinitionVertexList and t3dGet3DSpriteDefinitionNormalList copy out of
// (no include/ struct covers it): the count shared by the two 0xc-byte-element lists at +0x18 and +0x1C. Not the
// refcounted definition a sprite points at from +0x2C, whose +0x14/+0x18 hold its volume and its frame array
// (see t3dUndefine3DSprite).
typedef struct T3D3DSpriteDefinitionVertexList_guess
{
    unsigned char unknown_00[0x14]; // 0x00
    unsigned int vertex_count;      // 0x14: vertices in the list, 0xc bytes each (normals share it)
    void *vertex_list;              // 0x18: the list t3dGet3DSpriteDefinitionVertexList copies out
    void *normal_list;              // 0x1c: the list t3dGet3DSpriteDefinitionNormalList copies out
} T3D3DSpriteDefinitionVertexList_guess;

// A 0x2C-byte element of the per-sprite element array at +0x24 (no include/ struct covers it).
typedef struct T3D3DSpriteElement_guess
{
    unsigned char unknown_00[0x08]; // 0x00
    void *buffer;                   // 0x08: freed with the owning sprite
    unsigned char unknown_0c[0x20]; // 0x0c: pads the element to its 0x2C-byte stride
} T3D3DSpriteElement_guess;

// The 3D sprite object t3dDestroy3DSprite and t3dUndefine3DSprite free (no include/ struct covers it).
typedef struct T3D3DSprite_guess
{
    unsigned int type;              // 0x00: 0xC for a valid 3D sprite (t3dIs3DSpriteValid, 0x1000887E)
    unsigned char unknown_04[0x04]; // 0x04
    void *entry;                    // 0x08: dictionary entry, removed and freed
    unsigned char flags;            // 0x0c: bit 0 = the entry is registered in the dictionary
    unsigned char unknown_0d[0x03]; // 0x0d
    int state;                      // 0x10: 0 = defined (destroy), 1 = undefining, else T3D_ERROR
    void *volume;                   // 0x14: passed to t3dDestroyVolume
    T3D3DSpriteFrame_guess *frames; // 0x18: owned frame array, 0 = use the definition's
    void *buffer;                   // 0x1c: freed when non-null
    unsigned int count;             // 0x20: number of frames and elements
    T3D3DSpriteElement_guess *elements; // 0x24: array of 0x2C-byte elements, each with a buffer at +0x08
    unsigned int palette;           // 0x28: what t3dSet3DSpritePalette stores
    T3D3DSpriteDefinition_guess *definition; // 0x2c: shared definition
} T3D3DSprite_guess;

// The 3D sprite record t3dGet3DSpriteCenterOffset reads: its 12-byte center offset sits at +0x1C, the slot the
// generic t3dGetSpriteCenterOffset (0x1001B263) also reads for a type-0xC sprite (no include/ struct covers it).
typedef struct T3D3DSpriteDef_guess
{
    unsigned char unknown_00[0x1c];  // 0x00
    EQGfxVector3_guess centerOffset; // 0x1c: the 12 bytes the getter copies out
} T3D3DSpriteDef_guess;

// The renderer t3dSet3DSpritePolygonRenderMethod reads (no include/ struct covers it): the count the index form of a
// render method (bit 0x80000000 set, its low 31 bits) must stay below.
typedef struct T3DRenderer_guess
{
    unsigned char unknown_00[0x3dc];  // 0x00
    unsigned int render_method_count; // 0x3dc: the render method count the index form is checked against
} T3DRenderer_guess;

// FUNCTION: EQGFX 0x10007EF3
// Reads the drawing pen assigned to a 2D sprite.
// __cdecl, one pointer argument (the callee pops 0). Returns the byte at +0x32 of the object in AL (the callers read
// AL only). The decompiler's signature is int ptr, read at [esp+4]; nothing else is touched.
unsigned char __cdecl t3dGet2DSpritePen_guess(int ptr)
{
    return *(unsigned char *)(ptr + 0x32);
}

// FUNCTION: EQGFX 0x10007EFB
// Changes the drawing pen assigned to a 2D sprite.
// __cdecl, two arguments (the callee pops 0): stores the pen byte at +0x32 of the 2D sprite and returns 0 in EAX
// (the callers read EAX); nothing else is touched.
int __cdecl t3dSet2DSpritePen(T3D2DSprite_guess *sprite, unsigned char pen)
{
    sprite->pen = pen;
    return 0;
}

// FUNCTION: EQGFX 0x10007F09
// Reads how many animation frames a 2D sprite skips between updates.
// __cdecl, one pointer argument (the callee pops 0): returns the dword at +0x74 of the 2D sprite in EAX (the
// callers read EAX); nothing else is touched.
int __cdecl t3dGet2DSpriteSkipFrames(T3D2DSprite_guess *sprite)
{
    return sprite->skip_frames;
}

// FUNCTION: EQGFX 0x10007F11
// Sets how many animation frames a 2D sprite skips between updates.
// __cdecl, two arguments (the callee pops 0): stores the skip-frame dword at +0x74 of the 2D sprite and returns 0
// in EAX (the callers read EAX); nothing else is touched.
int __cdecl t3dSet2DSpriteSkipFrames(T3D2DSprite_guess *sprite, int skip_frames)
{
    sprite->skip_frames = skip_frames;
    return 0;
}

// FUNCTION: EQGFX 0x10007F1F
// Reads the palette assigned to a 2D sprite.
// __cdecl, one pointer argument (the callee pops 0): returns the dword at +0x84 of the 2D sprite in EAX (the
// callers read EAX, reaching it through the table at 0x1007C434); nothing else is touched.
unsigned int __cdecl t3dGet2DSpritePalette(T3D2DSprite_guess *sprite)
{
    return sprite->palette;
}

// FUNCTION: EQGFX 0x10007F2A
// Changes the palette assigned to a 2D sprite.
// __cdecl, two arguments (the callee pops 0): stores the palette dword at +0x84 of the 2D sprite and returns 0 in
// EAX (the callers read EAX); nothing else is touched.
int __cdecl t3dSet2DSpritePalette(T3D2DSprite_guess *sprite, unsigned int palette)
{
    sprite->palette = palette;
    return 0;
}

// FUNCTION: EQGFX 0x10007F3B
// Reads the bounding radius of a 2D sprite definition.
// __cdecl, one pointer argument (the callee pops 0): returns the float at +0x2C of the 2D sprite definition in
// ST0 (the callers read ST0); EAX only holds the argument, nothing else is touched.
float __cdecl t3dGet2DSpriteDefinitionBoundingRadius(T3D2DSpriteDefinition_guess *definition)
{
    return definition->bounding_radius;
}

// FUNCTION: EQGFX 0x10007F43
// Pauses or resumes a 2D sprite's animation.
// __cdecl (three stack arguments; the callee pops 0). `suspend` != 0 sets bit 0x08 of the 2D sprite's flags, 0
// clears it and ORs `resume_flags` into the dword at +0x70; returns 0.
int __cdecl t3dSuspend2DSpriteAnimation(T3D2DSprite_guess *sprite, int suspend, unsigned int resume_flags)
{
    if (suspend != 0)
        sprite->flags |= T3D2D_SPRITE_FLAG_SUSPENDED;
    else
        sprite->flags &= ~T3D2D_SPRITE_FLAG_SUSPENDED;
    if (suspend == 0)
        sprite->anim_flags |= resume_flags;
    return 0;
}

// FUNCTION: EQGFX 0x10007F69
int * __cdecl t3dDefineGeometryResource_guess(void *dictionary, char *name, unsigned int vertex_count, void *currentVertex,
                           int group_count, T3DGeometrySourceRecord_guess *polygonRecords,
                           unsigned int generate_normals, unsigned int *origin,
                           T3DGeometryResourceReference_guess *resource_references,
                           unsigned int instance_value, float *normalCursor)
{
    T3DGeometryResource_guess *resource;
    T3DGeometrySourceRecord_guess *source_record;
    T3DGeometrySourceRecord_guess *source_record_end;
    T3DGeometryInstance_guess *instance;
    T3DGeometryResourceReference_guess *reference_record;
    T3DGeometryResourceReference_guess *reference_end;
    unsigned int *vertex_index;
    unsigned int *vertex_index_end;
    unsigned int byte_count;
    unsigned int matched_group_count;
    float normal_x;
    float normal_y;
    float normal_z;
    float normal_scale;
    void *position_buffer;
    int result;

    resource = (T3DGeometryResource_guess *)EQ_CDECL(void *, 0x1000E7FE, (void *, char *))(dictionary, name);
    if (resource != 0)
    {
        if (resource->type != T3D_GEOMETRY_RESOURCE_TYPE)
            return 0;
        return (int *)resource;
    }
    resource = (T3DGeometryResource_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(sizeof(T3DGeometryResource_guess));
    if (resource == 0)
        return 0;
    resource->flags = 0;
    resource->group_count = group_count;
    resource->type = T3D_GEOMETRY_RESOURCE_TYPE;
    resource->ref_count = 1;
    resource->vertex_count = vertex_count;
    resource->source_user_data = normalCursor;
    byte_count = EQ_CDECL(unsigned int, 0x10068160, (char *))(name) + 1;
    resource->name = (char *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(byte_count);
    if (resource->name == 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(resource);
        return 0;
    }
    EQ_CDECL(void, 0x10068070, (char *, char *))(resource->name, name);
    byte_count = vertex_count * 0x0c;
    position_buffer = EQ_CDECL(void *, 0x10050C48, (unsigned int))(byte_count);
    if (position_buffer == 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(resource->name);
        EQ_CDECL(void, 0x10050CBB, (void *))(resource);
        return 0;
    }
    resource->vertex_positions = position_buffer;
    EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(position_buffer, currentVertex, byte_count);
    if (generate_normals == 0)
    {
        resource->vertex_normals = 0;
    }
    else
    {
        normalCursor = (float *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(byte_count);
        resource->vertex_normals = normalCursor;
        if (normalCursor == 0)
            goto cleanup_allocations;
        source_record_end = polygonRecords + group_count;
        currentVertex = 0;
        if (vertex_count != 0)
        {
            do
            {
                normal_x = 0.0f;
                matched_group_count = 0;
                normal_y = 0.0f;
                normal_z = 0.0f;
                if (polygonRecords < source_record_end)
                {
                    source_record = polygonRecords;
                    do
                    {
                        vertex_index = source_record->vertex_indices;
                        vertex_index_end = vertex_index + source_record->vertex_index_count;
                        for (; vertex_index < vertex_index_end; vertex_index = vertex_index + 1)
                        {
                            if ((void *)*vertex_index == currentVertex)
                            {
                                normal_x = normal_x + source_record->normal_x;
                                matched_group_count = matched_group_count + 1;
                                normal_y = normal_y + source_record->normal_y;
                                normal_z = normal_z + source_record->normal_z;
                                break;
                            }
                        }
                        source_record = source_record + 1;
                    } while (source_record < source_record_end);
                    if (matched_group_count < 2)
                        goto store_unscaled_normal;
                    normal_scale = T3D_GEOMETRY_LENGTH_SCALE_FUNCTION(
                        normal_x * normal_x + normal_y * normal_y + normal_z * normal_z);
                    normalCursor[0] = normal_scale * normal_x;
                    normalCursor[1] = normal_scale * normal_y;
                    normalCursor[2] = normal_scale * normal_z;
                }
                else
                {
store_unscaled_normal:
                    normalCursor[0] = normal_x;
                    normalCursor[1] = normal_y;
                    normalCursor[2] = normal_z;
                }
                normalCursor = normalCursor + 3;
                currentVertex = (void *)((unsigned int)currentVertex + 1);
            } while ((unsigned int)currentVertex < vertex_count);
        }
    }
    source_record_end = polygonRecords + group_count;
    for (source_record = polygonRecords; source_record < source_record_end; source_record = source_record + 1)
        source_record->vertex_positions = position_buffer;
    result = EQ_CDECL(int, 0x1001564B, (void *, void *))(dictionary, resource);
    if (result == 0)
    {
        result = EQ_CDECL(int, 0x1000DC08, (void *, void *))(dictionary, resource);
        if (result < 0)
        {
            EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, resource);
        }
        else
        {
            resource->source_groups = polygonRecords;
            instance = (T3DGeometryInstance_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(sizeof(T3DGeometryInstance_guess));
            if (instance == 0)
            {
                EQ_CDECL(void, 0x1000E334, (void *, char *))(dictionary, name);
                EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, resource);
            }
            else
            {
                resource->instance = instance;
                instance->reserved_08 = 0;
                instance->reserved_0c = 0;
                instance->type = T3D_GEOMETRY_INSTANCE_TYPE;
                instance->ref_count = 1;
                instance->origin_x = origin[0];
                instance->origin_y = origin[1];
                instance->origin_z = origin[2];
                instance->reserved_28 = 0;
                instance->resource = resource;
                instance->instance_value = instance_value;
                instance->output_records = resource_references;
                result = EQ_CDECL(int, 0x1001564B, (void *, void *))(dictionary, instance);
                if (result == 0)
                {
                    reference_end = resource_references + group_count;
                    while ((reference_record = reference_end - 1), resource_references < reference_end)
                    {
                        unsigned int *shared_resource;

                        shared_resource = reference_record->shared_resource;
                        reference_record->type = T3D_GEOMETRY_REFERENCE_TYPE;
                        reference_end = reference_record;
                        if (shared_resource != 0 && *shared_resource != 0)
                            EQ_CDECL(void, 0x10017CE8, (void *))(shared_resource);
                    }
                    return (int *)resource;
                }
                EQ_CDECL(void, 0x10050CBB, (void *))(instance);
                EQ_CDECL(void, 0x1000E334, (void *, char *))(dictionary, name);
                EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, resource);
            }
        }
    }
    if (resource->vertex_normals != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(resource->vertex_normals);
cleanup_allocations:
    EQ_CDECL(void, 0x10050CBB, (void *))(position_buffer);
    EQ_CDECL(void, 0x10050CBB, (void *))(resource->name);
    EQ_CDECL(void, 0x10050CBB, (void *))(resource);
    return 0;
}

// FUNCTION: EQGFX 0x10008262
// Removes a 3D sprite and its definition when it is the definition's last user.
// __cdecl (two stack arguments; the callee pops 0). Undefines a 3D sprite whose shared definition is down to its
// last user and returns the dictionary's result; any other state returns 0xFFFF3D44.
int __cdecl t3dUndefine3DSprite(void *world, T3D3DSprite_guess *sprite)
{
    int result;
    T3D3DSpriteDefinition_guess *definition;
    T3D3DSpriteFrame_guess *end;
    T3D3DSpriteElement_guess *element;

    definition = sprite->definition;
    if (sprite->state == 1 && definition->refcount == 1)
    {
        EQ_CDECL(void, 0x1001D5A1, (void *, void *))(world, definition->volume);
        if ((definition->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(world, definition->entry);
        }
        EQ_CDECL(int, 0x10015D81, (void *, void *))(world, definition);
        end = definition->frames + sprite->count;
        while (end-- > definition->frames)
        {
            if (end->simple_sprite != 0 && *end->simple_sprite != 0)
            {
                EQ_CDECL(void, 0x10017DAD, (void *))(end->simple_sprite);
                EQ_CDECL(void, 0x1001B94A, (void *, void *))(world, end->simple_sprite);
            }
            if (end->buffer != 0)
            {
                EQ_CDECL(void, 0x10050CBB, (void *))(end->buffer);
            }
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(definition->frames);
        if (definition->entry != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(definition->entry);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(definition);
        if ((sprite->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(world, sprite->entry);
        }
        result = EQ_CDECL(int, 0x10015D81, (void *, void *))(world, sprite);
        element = sprite->elements + sprite->count;
        while (element-- > sprite->elements)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(element->buffer);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(sprite->elements);
        if (sprite->buffer != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite->buffer);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(sprite->frames);
        if (sprite->entry != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite->entry);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
    }
    else
    {
        result = 0xffff3d44;
    }
    return result;
}

// FUNCTION: EQGFX 0x100083A4
// Checks whether a 3D sprite definition and its parent are valid.
// __cdecl, one stack argument (the callee pops 0). Returns 1 for a type-0xB definition whose +0x2C parent is a
// type-0xC definition, 0 for any null or mistyped one (the sibling of t3dIs2DSpriteDefinitionValid).
int __cdecl t3dIs3DSpriteDefinitionValid(T3D3DSpriteDefinition_guess *definition)
{
    int is_valid;

    is_valid = 1;
    if (definition == 0 || definition->parent == 0 || definition->type != 11 || definition->parent->type != 12)
        is_valid = 0;
    return is_valid;
}

// FUNCTION: EQGFX 0x100083C3
// Reads the vertex positions that define a 3D sprite mesh.
// __cdecl, two stack arguments (the callee pops 0): copies the definition's 0xc-byte vertex list into the
// caller's buffer and returns 0 in EAX (the callers read EAX); nothing else is touched.
int __cdecl t3dGet3DSpriteDefinitionVertexList(T3D3DSpriteDefinitionVertexList_guess *definition, void *vertices)
{
    EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))(vertices, definition->vertex_list,
                                                                       definition->vertex_count * 0x0c);
    return 0;
}

// FUNCTION: EQGFX 0x100083E3
// Reads the surface normals for a 3D sprite mesh.
// __cdecl, two stack arguments (the callee pops 0). Copies the definition's 0xc-byte normal list into the
// caller's buffer and returns 0, or 0xFFFF3D3E when the list pointer at +0x1C is null; nothing else is touched.
int __cdecl t3dGet3DSpriteDefinitionNormalList(T3D3DSpriteDefinitionVertexList_guess *definition, void *normals)
{
    int result;
    void *normal_list;

    result = 0;
    normal_list = definition->normal_list;
    if (normal_list == 0)
    {
        result = 0xffff3d3e;
    }
    else
    {
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))(normals, normal_list,
                                                                           definition->vertex_count * 0x0c);
    }
    return result;
}

// FUNCTION: EQGFX 0x10008413
// Gets the number of corners in a 3D sprite polygon.
// __cdecl, two arguments (the callee pops 0). Returns the vertex count at +0x04 of the polygon record `index` of
// the 3D sprite definition's 0x2C-byte polygon array at +0x24, or 0 when index is not below the count at +0x20.
unsigned int __cdecl t3dGet3DSpriteDefinitionPolygonNumVertices(T3D3DSpriteDefinition_guess *definition, unsigned int index)
{
    if (index >= (unsigned int)definition->frame_count)
    {
        return 0;
    }
    return definition->polygons[index].vertex_count;
}

// FUNCTION: EQGFX 0x1000842E
// Gets the vertex references that define a 3D sprite polygon.
// __cdecl, three arguments (the callee pops 0). Copies polygon `index`'s vertex index list (vertex count at polygon
// +0x04, list at polygon +0x08) into `dest`; an index not below the polygon count at +0x20 returns 0xFFFF3D3E.
int __cdecl t3dGet3DSpriteDefinitionPolygonVertexIndexList(T3D3DSpriteDefinition_guess *definition, unsigned int index, void *dest)
{
    int result;

    result = 0;
    if (index >= definition->frame_count)
    {
        result = 0xffff3d3e;
    }
    else
    {
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
            (dest, definition->polygons[index].vertex_indices, definition->polygons[index].vertex_count << 2);
    }
    return result;
}

// FUNCTION: EQGFX 0x10008467
// Gets the positions of the vertices that form a 3D sprite polygon.
// __cdecl, three arguments (the callee pops 0). Expands polygon `index`'s vertex index list (vertex count at polygon
// +0x04, list at +0x08) into the 0xc-byte vertices `vertices`, indexing the definition's list at +0x18; an index not
// below the polygon count at +0x20 returns 0xFFFF3D3E.
int __cdecl t3dGet3DSpriteDefinitionPolygonVertexList(T3D3DSpriteDefinition_guess *definition, unsigned int index, EQGfxVector3_guess *vertices)
{
    int result;
    T3D3DSpritePolygon_guess *polygon;
    EQGfxVector3_guess *vertex_list;
    int *vertex_index;
    EQGfxVector3_guess *vertex;
    EQGfxVector3_guess *end;

    result = 0;
    if (index >= (unsigned int)definition->frame_count)
    {
        result = 0xffff3d3e;
    }
    else
    {
        polygon = &definition->polygons[index];
        vertex_list = (EQGfxVector3_guess *)definition->frames;
        vertex_index = polygon->vertex_indices;
        end = vertices + polygon->vertex_count;
        vertex = vertices;
        while (vertex < end)
        {
            *vertex++ = vertex_list[*vertex_index++];
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x100084C3
// Gets the plane describing a 3D sprite polygon's face.
// __cdecl, three arguments (the callee pops 0). Copies the plane (type 0x2F, normal at +0x10, distance at +0x1C)
// of the 0x2C-byte polygon record `index` into `plane`; an index not below the count at +0x20 returns 0xFFFF3D3E.
int __cdecl t3dGet3DSpriteDefinitionPolygonPlane(T3D3DSpriteDefinition_guess *definition, unsigned int index, T3DPlane_guess *plane)
{
    int result;
    T3D3DSpritePolygon_guess *polygon;

    result = 0;
    if (index >= (unsigned int)definition->frame_count)
    {
        result = 0xffff3d3e;
    }
    else
    {
        polygon = &definition->polygons[index];
        plane->type = 0x2f;
        plane->normal = polygon->normal;
        plane->distance = polygon->distance;
    }
    return result;
}

// FUNCTION: EQGFX 0x100084FF
// Creates a 3D sprite instance from a shared definition, optionally using a supplied volume.
T3D3DSprite_guess *__cdecl t3dCreate3DSprite(void *context, T3D3DSpriteDefinition_guess *definition, void *volume)
{
    T3D3DSprite_guess *sprite;
    T3D3DSpriteDefinition_guess *parent;
    T3D3DSpriteFrame_guess *frame_start;
    T3D3DSpriteFrame_guess *frame_end;
    unsigned int *source_words;
    unsigned int *destination_words;
    unsigned int word_count;
    int *simple_sprite;
    void *duplicate_volume;

    sprite = (T3D3DSprite_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x30);
    if (sprite != 0)
    {
        parent = definition->parent;
        source_words = (unsigned int *)parent;
        destination_words = (unsigned int *)sprite;
        for (word_count = 0x0c; word_count != 0; word_count--)
        {
            *destination_words = *source_words;
            source_words++;
            destination_words++;
        }
        *((unsigned int *)&sprite->flags) &= 0xc0000000;
        sprite->entry = 0;
        sprite->state = 0;
        sprite->frames = 0;
        if (volume == 0)
        {
            volume = parent->volume;
        }
        duplicate_volume = EQ_CDECL(void *, 0x1001D516, (void *, void *))(context, volume);
        sprite->volume = duplicate_volume;
        if (duplicate_volume == 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
            sprite = 0;
        }
        else if (EQ_CDECL(int, 0x1001564B, (void *, void *))(context, sprite) != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite->frames);
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
            sprite = 0;
        }
        else
        {
            frame_start = parent->frames;
            frame_end = frame_start + definition->frame_count;
            while (frame_start < frame_end)
            {
                frame_end--;
                simple_sprite = frame_end->simple_sprite;
                if (simple_sprite != 0 && *simple_sprite != 0)
                {
                    EQ_CDECL(void, 0x10017CE8, (void *))(simple_sprite);
                }
            }
            definition->refcount++;
        }
    }
    return sprite;
}

// FUNCTION: EQGFX 0x100085C8
// Duplicates an existing 3D sprite, preserving its individual polygon settings.
T3D3DSprite_guess *__cdecl t3dDuplicate3DSprite(void *context, T3D3DSprite_guess *source_sprite, void *volume_override)
{
    T3D3DSprite_guess *sprite;
    T3D3DSpriteDefinition_guess *definition;
    T3D3DSpriteFrame_guess *frame_base;
    T3D3DSpriteFrame_guess *frame;
    T3D3DSpriteFrame_guess *frame_end;
    T3D3DSpritePolygon_guess *polygon;
    unsigned int *source_words;
    unsigned int *destination_words;
    unsigned int word_count;
    unsigned int frame_byte_count;
    unsigned int buffer_byte_count;
    void *buffer;

    sprite = (T3D3DSprite_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x30);
    if (sprite == 0)
    {
        return 0;
    }
    source_words = (unsigned int *)source_sprite;
    destination_words = (unsigned int *)sprite;
    for (word_count = 0x0c; word_count != 0; word_count--)
    {
        *destination_words = *source_words;
        source_words++;
        destination_words++;
    }
    *((unsigned int *)&sprite->flags) &= 0xc0000000;
    definition = sprite->definition;
    sprite->entry = 0;
    sprite->state = 0;
    if (volume_override == 0)
    {
        volume_override = source_sprite->volume;
    }
    sprite->volume = EQ_CDECL(void *, 0x1001D516, (void *, void *))(context, volume_override);
    if (sprite->volume == 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
        sprite = 0;
    }
    else
    {
        if (source_sprite->frames == 0)
        {
            sprite->frames = 0;
        }
        else
        {
            frame_byte_count = definition->frame_count * 0x50;
            sprite->frames = (T3D3DSpriteFrame_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(frame_byte_count);
            if (sprite->frames == 0)
            {
                EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context, sprite->volume);
                EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
                return 0;
            }
            EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
                (sprite->frames, source_sprite->frames, frame_byte_count);
        }
        if (EQ_CDECL(int, 0x1001564B, (void *, void *))(context, sprite) == 0)
        {
            frame_base = sprite->frames;
            if (frame_base == 0)
            {
                frame_base = definition->parent->frames;
            }
            frame_end = frame_base + definition->frame_count;
            frame = frame_end - 1;
            if ((unsigned int)frame_end > (unsigned int)sprite->frames)
            {
                polygon = definition->polygons + definition->frame_count;
                do
                {
                    polygon--;
                    if (frame->buffer != 0 && sprite->frames != 0)
                    {
                        buffer_byte_count = polygon->vertex_count << 3;
                        if (buffer_byte_count == 0)
                        {
                            frame->buffer = 0;
                        }
                        else
                        {
                            buffer = EQ_CDECL(void *, 0x10050C48, (unsigned int))(buffer_byte_count);
                            if (buffer == 0)
                            {
                                frame--;
                                while ((unsigned int)frame >= (unsigned int)sprite->frames)
                                {
                                    if (frame->simple_sprite != 0 && *frame->simple_sprite != 0)
                                    {
                                        EQ_CDECL(void, 0x10017DAD, (void *))(frame->simple_sprite);
                                    }
                                    if (frame->buffer != 0)
                                    {
                                        EQ_CDECL(void, 0x10050CBB, (void *))(frame->buffer);
                                    }
                                    frame--;
                                }
                                EQ_CDECL(int, 0x10015D81, (void *, void *))(context, sprite);
                                EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context, sprite->volume);
                                EQ_CDECL(void, 0x10050CBB, (void *))(sprite->frames);
                                EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
                                return 0;
                            }
                            EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
                                (buffer, frame->buffer, buffer_byte_count);
                            frame->buffer = buffer;
                        }
                    }
                    if (frame->simple_sprite != 0 && *frame->simple_sprite != 0)
                    {
                        EQ_CDECL(void, 0x10017CE8, (void *))(frame->simple_sprite);
                    }
                } while ((unsigned int)frame-- > (unsigned int)sprite->frames);
            }
            sprite->definition->refcount++;
        }
        else
        {
            EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context, sprite->volume);
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite->frames);
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
            sprite = 0;
        }
    }
    return sprite;
}

// FUNCTION: EQGFX 0x1000879D
// Removes a 3D sprite instance while retaining its shared definition for reuse.
// __cdecl (two stack arguments; the callee pops 0). Destroys a defined 3D sprite and returns the dictionary's
// result; any other state returns 0xFFFF3D45.
int __cdecl t3dDestroy3DSprite(void *dictionary, T3D3DSprite_guess *sprite)
{
    int result;
    T3D3DSpriteFrame_guess *frame;
    T3D3DSpriteFrame_guess *end;

    if (sprite->state != 0)
    {
        result = 0xffff3d45;
    }
    else
    {
        sprite->definition->refcount--;
        EQ_CDECL(void, 0x1001D5A1, (void *, void *))(dictionary, sprite->volume);
        if ((sprite->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, sprite->entry);
        }
        result = EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, sprite);
        frame = sprite->frames;
        if (frame == 0)
        {
            frame = sprite->definition->parent->frames;
        }
        end = frame + sprite->definition->frame_count;
        while (end-- > frame)
        {
            if (end->simple_sprite != 0 && *end->simple_sprite != 0)
            {
                EQ_CDECL(void, 0x10017DAD, (void *))(end->simple_sprite);
                EQ_CDECL(void, 0x1001B94A, (void *, void *))(dictionary, end->simple_sprite);
            }
            if (end->buffer != 0 && sprite->frames != 0)
            {
                EQ_CDECL(void, 0x10050CBB, (void *))(end->buffer);
            }
        }
        if (sprite->frames != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite->frames);
        }
        if (sprite->entry != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite->entry);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
    }
    return result;
}

// FUNCTION: EQGFX 0x10008874
// Keeps a 2D sprite available for another user.
// __cdecl, one pointer argument (the callee pops 0). Increments the share count at +0x10 of the 2D sprite and
// returns 0 (the callers read EAX).
int __cdecl t3dShare2DSprite(T3D2DSprite_guess *sprite)
{
    ++sprite->ref_count;
    return 0;
}

// FUNCTION: EQGFX 0x1000887E
// Checks whether a 3D sprite has a valid shared definition.
// __cdecl, one pointer argument (the callee pops 0). Returns 1 in EAX for a type-12 3D sprite whose shared definition
// at +0x2C is present and type 11, 0 for any null or mistyped one (the sibling of t3dIs2DSpriteValid).
int __cdecl t3dIs3DSpriteValid(T3D3DSprite_guess *sprite)
{
    int is_valid;

    is_valid = 1;
    if (sprite == 0 || sprite->definition == 0 || sprite->definition->type != 11 || sprite->type != 12)
        is_valid = 0;
    return is_valid;
}

// FUNCTION: EQGFX 0x1000889D
// Reads a 3D sprite's center offset.
// __cdecl, two arguments (the callee pops 0): copies the 12-byte center offset at +0x1C of the 3D sprite record
// into the caller's vector and returns 0 in EAX (the callers read EAX); nothing else is touched.
int __cdecl t3dGet3DSpriteCenterOffset(T3D3DSpriteDef_guess *sprite, EQGfxVector3_guess *centerOffset)
{
    *centerOffset = sprite->centerOffset;
    return 0;
}

// FUNCTION: EQGFX 0x100088B2
// Changes a 3D sprite's center offset.
// __cdecl, two arguments (the callee pops 0): copies the 12-byte center offset into +0x1C of the 3D sprite
// record and returns 0 in EAX (the callers read EAX); nothing else is touched.
int __cdecl t3dSet3DSpriteCenterOffset(T3D3DSpriteDef_guess *sprite, EQGfxVector3_guess *centerOffset)
{
    sprite->centerOffset = *centerOffset;
    return 0;
}

// FUNCTION: EQGFX 0x100088C7
// Changes the palette assigned to a 3D sprite.
// __cdecl, two arguments (the callee pops 0): stores the palette dword at +0x28 of the 3D sprite and returns 0 in
// EAX (the callers read EAX); nothing else is touched.
int __cdecl t3dSet3DSpritePalette(T3D3DSprite_guess *sprite, unsigned int palette)
{
    sprite->palette = palette;
    return 0;
}

// FUNCTION: EQGFX 0x100088D5
// Gets the rendering method assigned to a 3D sprite frame.
// __cdecl, two arguments (the callee pops 0). Returns the render method at +0x04 of frame[frameIndex]; the frame array
// is the sprite's own when it has one, else the definition's parent's; 0 when frameIndex is not below the frame count.
int __cdecl t3dGet3DSpritePolygonRenderMethod(T3D3DSprite_guess *sprite, unsigned int frameIndex)
{
    T3D3DSpriteFrame_guess *frame;

    if (frameIndex >= (unsigned int)sprite->definition->frame_count)
    {
        return 0;
    }
    frame = sprite->frames;
    if (frame == 0)
    {
        frame = sprite->definition->parent->frames;
    }
    return frame[frameIndex].render_method;
}

#define T3D_ERROR_SET_RENDER_METHOD 0xffff3d3e

// FUNCTION: EQGFX 0x10008900
// Assigns a rendering method to one 3D sprite frame.
// __cdecl, four arguments (the callee pops 0). Stores renderMethod at +0x04 of frame[frameIndex] when frameIndex is below the
// frame count and renderMethod is valid; a sprite with no frame array gets one from t3dCopy3DSpriteDefinitionFrames_guess first. Returns 0 or the error.
int __cdecl t3dSet3DSpritePolygonRenderMethod(T3DRenderer_guess *renderer, T3D3DSprite_guess *sprite, unsigned int frameIndex, unsigned int renderMethod)
{
    int result = 0;
    int invalidRenderMethod = frameIndex >= (unsigned int)sprite->definition->frame_count
        || ((renderMethod & 0x80000000) != 0 && (renderMethod & 0x7fffffff) >= renderer->render_method_count)
        || ((renderMethod & 0x80000000) == 0 && (renderMethod & 0xfeefffff) > 0x7fffffff)
        || (sprite->frames == 0 && EQ_CDECL(int, 0x1000897D, (T3D3DSprite_guess *))(sprite) != 0);
    if (invalidRenderMethod)
    {
        result = T3D_ERROR_SET_RENDER_METHOD;
    }
    else
    {
        sprite->frames[frameIndex].render_method = renderMethod;
    }
    return result;
}

#define T3D_ERROR_MEMORY_ALLOCATION 0xffff3d3e

// FUNCTION: EQGFX 0x1000897D
// Gives a 3D sprite its own polygon settings so appearance changes affect that instance independently.
// @early-stop: the loop and stack layout still differ (52 instruction differences remain).
/* fake match: volatile pointers keep the array and polygon cursors in stack slots. */
int __cdecl t3dCopy3DSpriteDefinitionFrames_guess(T3D3DSprite_guess *sprite)
{
    int result;
    T3D3DSpriteDefinition_guess *definition;
    T3D3DSpriteFrame_guess * volatile frameArray;
    T3D3DSpriteFrame_guess *frameEnd;
    T3D3DSpriteFrame_guess *frame;
    int * volatile polygonVertexCount;
    unsigned int frameArraySize;
    unsigned int bufferSize;
    void *buffer;

    result = 0;
    definition = sprite->definition;
    frameArraySize = definition->frame_count * 0x50;
    frameArray = (T3D3DSpriteFrame_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))
        (frameArraySize);
    if (frameArray != 0)
    {
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
            (frameArray, definition->parent->frames, frameArraySize);
        polygonVertexCount = &definition->polygons->vertex_count;
        frame = frameArray;
        frameEnd = frameArray + definition->frame_count;
        if (frameArray < frameEnd)
        {
            do
            {
                if (frame->buffer != 0)
                {
                    bufferSize = *polygonVertexCount << 3;
                    buffer = EQ_CDECL(void *, 0x10050C48, (unsigned int))(bufferSize);
                    if (buffer == 0)
                    {
                        frame = frame - 1;
                        if (frame >= frameArray)
                        {
                            if (frame->buffer != 0)
                            {
                                EQ_CDECL(void, 0x10050CBB, (void *))(frame->buffer);
                            }
                            EQ_CDECL(void, 0x10050CBB, (void *))(frameArray);
                            result = T3D_ERROR_MEMORY_ALLOCATION;
                            goto done;
                        }
                    }
                    EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))
                        (buffer, frame->buffer, bufferSize);
                    frame->buffer = buffer;
                }
                polygonVertexCount += 11;
                frame = frame + 1;
            } while (frame < frameEnd);
        }
        sprite->frames = frameArray;
    }
    else
    {
        result = T3D_ERROR_MEMORY_ALLOCATION;
    }

done:
    return result;
}

// FUNCTION: EQGFX 0x10008A4C
// Gets the constant intensity value for a 3D sprite frame.
// __cdecl, two arguments (the callee pops 0). Returns the frame's intensity float at +0x08; the frame array is the
// sprite's own when it has one, else the definition's parent's; 0.0 when frameIndex is not below the frame count.
float __cdecl t3dGet3DSpritePolygonConstantIntensity(T3D3DSprite_guess *sprite, unsigned int frameIndex)
{
    T3D3DSpriteFrame_guess *frame;
    T3D3DSpriteDefinition_guess *definition;
    float intensity;

    definition = sprite->definition;
    if (frameIndex >= (unsigned int)definition->frame_count)
    {
        intensity = 0;
    }
    else
    {
        frame = sprite->frames;
        if (frame == 0)
        {
            frame = definition->parent->frames;
        }
        intensity = frame[frameIndex].intensity;
    }
    return intensity;
}

// FUNCTION: EQGFX 0x10008A77
// Sets the constant intensity value for a 3D sprite frame.
// __cdecl, three arguments (the callee pops 0). Stores intensity at frame[frameIndex]'s +0x08 and returns 0, or 0xffff3d3e
// when frameIndex is not below the frame count or the sprite has no frame array and t3dCopy3DSpriteDefinitionFrames_guess returns nonzero.
int __cdecl t3dSet3DSpritePolygonConstantIntensity(T3D3DSprite_guess *sprite, unsigned int frameIndex, float intensity)
{
    T3D3DSpriteFrame_guess *frame;
    T3D3DSpriteDefinition_guess *definition;
    int result;

    result = 0;
    definition = sprite->definition;
    if (frameIndex < (unsigned int)definition->frame_count &&
        (sprite->frames != 0 || EQ_CDECL(int, 0x1000897D, (T3D3DSprite_guess *))(sprite) == 0))
    {
        frame = sprite->frames;
        frame[frameIndex].intensity = intensity;
    }
    else
    {
        result = 0xffff3d3e;
    }
    return result;
}

// FUNCTION: EQGFX 0x10008ABA
// Gets the ambient light scaling value for a 3D sprite frame.
// __cdecl, two arguments (the callee pops 0). Returns the frame's ambient-scaler float at +0x0C; the frame array is
// the sprite's own when it has one, else the definition's parent's; 0.0 when frameIndex is not below the frame count.
float __cdecl t3dGet3DSpritePolygonAmbientScaler(T3D3DSprite_guess *sprite, unsigned int frameIndex)
{
    T3D3DSpriteFrame_guess *frame;
    T3D3DSpriteDefinition_guess *definition;
    float ambient_scaler;

    definition = sprite->definition;
    if (frameIndex >= (unsigned int)definition->frame_count)
    {
        ambient_scaler = 0;
    }
    else
    {
        frame = sprite->frames;
        if (frame == 0)
        {
            frame = definition->parent->frames;
        }
        ambient_scaler = frame[frameIndex].ambient_scaler;
    }
    return ambient_scaler;
}

// FUNCTION: EQGFX 0x10008AE5
// Sets the ambient light scaling value for a 3D sprite frame.
// __cdecl, three arguments (the callee pops 0). Stores ambientScaler at the frame's +0x0C float and returns 0, or 0xffff3d3e when
// frameIndex is not below the frame count, or when the sprite has no frames of its own and the call to 0x1000897D returns nonzero.
int __cdecl t3dSet3DSpritePolygonAmbientScaler(T3D3DSprite_guess *sprite, unsigned int frameIndex, float ambientScaler)
{
    T3D3DSpriteFrame_guess *frame;
    T3D3DSpriteDefinition_guess *definition;
    int result;

    result = 0;
    definition = sprite->definition;
    if (frameIndex >= (unsigned int)definition->frame_count ||
        (sprite->frames == 0 && EQ_CDECL(int, 0x1000897D, (T3D3DSprite_guess *))(sprite) != 0))
    {
        result = 0xffff3d3e;
    }
    else
    {
        frame = sprite->frames;
        frame[frameIndex].ambient_scaler = ambientScaler;
    }
    return result;
}

// FUNCTION: EQGFX 0x10008B28
// Gets the pen assigned to a 3D sprite frame's polygons.
// __cdecl, two arguments (the callee pops 0). Returns the frame's pen byte at +0x1A in AL; the frame array is the
// sprite's own when it has one, else the definition's parent's; 0 when frameIndex is not below the frame count.
unsigned char __cdecl t3dGet3DSpritePolygonPen(T3D3DSprite_guess *sprite, unsigned int frameIndex)
{
    T3D3DSpriteFrame_guess *frame;
    T3D3DSpriteDefinition_guess *definition;

    definition = sprite->definition;
    if (frameIndex >= (unsigned int)definition->frame_count)
    {
        return 0;
    }
    frame = sprite->frames;
    if (frame == 0)
    {
        frame = definition->parent->frames;
    }
    return frame[frameIndex].pen;
}

// FUNCTION: EQGFX 0x10008B53
// Sets the pen assigned to a 3D sprite frame's polygons.
// __cdecl, three arguments (the callee pops 0). Stores pen at +0x1A of frame[frameIndex] and returns 0; a sprite with
// no frame array first calls 0x1000897D, and a nonzero result there or frameIndex not below the count returns 0xffff3d3e.
int __cdecl t3dSet3DSpritePolygonPen(T3D3DSprite_guess *sprite, unsigned int frameIndex, unsigned char pen)
{
    T3D3DSpriteFrame_guess *frame;
    T3D3DSpriteDefinition_guess *definition;
    int result;

    result = 0;
    definition = sprite->definition;
    if (frameIndex >= (unsigned int)definition->frame_count)
    {
        goto rejected;
    }
    if (sprite->frames != 0)
    {
        goto stored;
    }
    if (EQ_CDECL(int, 0x1000897D, (T3D3DSprite_guess *))(sprite) == 0)
    {
        goto stored;
    }
rejected:
    result = 0xffff3d3e;
    goto done;
stored:
    frame = sprite->frames;
    frame[frameIndex].pen = pen;
done:
    return result;
}

// FUNCTION: EQGFX 0x10008B96
// Gets the simple sprite attached to a 3D sprite frame.
// __cdecl, two arguments (the callee pops 0). Returns the frame's +0x1C simple-sprite pointer when it and its first
// dword are non-null; the frame array is the sprite's own when it has one, else the definition's parent's; else 0.
int *__cdecl t3dGet3DSpritePolygonSimpleSprite(T3D3DSprite_guess *sprite, unsigned int frameIndex)
{
    T3D3DSpriteFrame_guess *frame;
    T3D3DSpriteDefinition_guess *definition;
    int *simpleSprite;

    definition = sprite->definition;
    if (frameIndex < (unsigned int)definition->frame_count)
    {
        frame = sprite->frames;
        if (frame == 0)
        {
            frame = definition->parent->frames;
        }
        simpleSprite = frame[frameIndex].simple_sprite;
        if (simpleSprite != 0 && *simpleSprite != 0)
        {
            return simpleSprite;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x10008BC9
// __cdecl; replaces a frame's simple sprite, updating its share count, and returns the frame-index error on failure.
int __cdecl t3dSet3DSpritePolygonSimpleSprite(T3D3DSprite_guess *sprite, unsigned int frameIndex, int *simpleSprite)
{
    T3D3DSpriteFrame_guess *frame;
    int *oldSprite;
    int result = 0;
    int invalidFrame = frameIndex >= (unsigned int)sprite->definition->frame_count ||
        (sprite->frames == 0 && EQ_CDECL(int, 0x1000897D, (T3D3DSprite_guess *))(sprite) != 0);
    if (invalidFrame)
    {
        result = 0xffff3d3e;
    }
    else
    {
        frame = &sprite->frames[frameIndex];
        oldSprite = frame->simple_sprite;
        if (oldSprite != 0 && *oldSprite != 0)
        {
            EQ_CDECL(void, 0x10017DAD, (int *))(oldSprite);
        }
        frame->simple_sprite = simpleSprite;
        if (simpleSprite != 0 && *simpleSprite != 0)
        {
            EQ_CDECL(void, 0x10017CE8, (int *))(simpleSprite);
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10008C34
// Gets the texture coordinate origin for a 3D sprite frame.
// __cdecl, three arguments (the callee pops 0). Copies frame[frameIndex]'s 12-byte UV origin at +0x20 into `uvOrigin`
// and returns 0, or 0xffff3d3e when frameIndex is not below the frame count; the frame array follows the usual fallback.
int __cdecl t3dGet3DSpritePolygonUVOrigin(T3D3DSprite_guess *sprite, unsigned int frameIndex, EQGfxVector3_guess *uvOrigin)
{
    T3D3DSpriteFrame_guess *frame;
    T3D3DSpriteDefinition_guess *definition;
    int result;

    result = 0;
    definition = sprite->definition;
    if (frameIndex >= (unsigned int)definition->frame_count)
    {
        result = 0xffff3d3e;
    }
    else
    {
        frame = sprite->frames;
        if (frame == 0)
        {
            frame = definition->parent->frames;
        }
        *uvOrigin = frame[frameIndex].uv_origin;
    }
    return result;
}

// FUNCTION: EQGFX 0x10008C70
// Sets the texture coordinate origin for a 3D sprite frame.
// __cdecl, three arguments (the callee pops 0). Copies `uvOrigin` into frame[frameIndex]'s 12-byte UV origin at +0x20
// and returns 0, or 0xffff3d3e when frameIndex is not below the frame count or a sprite with no frame array gets none from 0x1000897D.
int __cdecl t3dSet3DSpritePolygonUVOrigin(T3D3DSprite_guess *sprite, unsigned int frameIndex, EQGfxVector3_guess *uvOrigin)
{
    T3D3DSpriteFrame_guess *frame;
    T3D3DSpriteDefinition_guess *definition;
    int result;
    int invalidFrame;

    result = 0;
    definition = sprite->definition;
    invalidFrame = frameIndex >= (unsigned int)definition->frame_count
        || (sprite->frames == 0 && EQ_CDECL(int, 0x1000897D, (T3D3DSprite_guess *))(sprite) != 0);
    if (invalidFrame)
    {
        result = 0xffff3d3e;
    }
    else
    {
        frame = sprite->frames;
        frame[frameIndex].uv_origin = *uvOrigin;
    }
    return result;
}

// FUNCTION: EQGFX 0x10008cb6
// __cdecl, three arguments (the callee pops 0). Copies frame[frameIndex]'s 12-byte U axis at +0x2c into `uvAxis`
// and returns 0, or 0xffff3d3e when frameIndex is not below the frame count; the frame array follows the usual fallback.
int __cdecl t3dGet3DSpritePolygonUAxis(T3D3DSprite_guess *sprite, unsigned int frameIndex, EQGfxVector3_guess *uvAxis)
{
    T3D3DSpriteFrame_guess *frame;
    T3D3DSpriteDefinition_guess *definition;
    int result;

    result = 0;
    definition = sprite->definition;
    if (frameIndex >= (unsigned int)definition->frame_count)
    {
        result = 0xffff3d3e;
    }
    else
    {
        frame = sprite->frames;
        if (frame == 0)
        {
            frame = definition->parent->frames;
        }
        *uvAxis = frame[frameIndex].u_axis;
    }
    return result;
}

// FUNCTION: EQGFX 0x10008cf2
// __cdecl, three arguments (the callee pops 0). Copies `uvAxis` into frame[frameIndex]'s 12-byte U axis at +0x2c
// and returns 0, or 0xffff3d3e when frameIndex is not below the frame count or a sprite with no frame array gets none from 0x1000897D.
int __cdecl t3dSet3DSpritePolygonUAxis(T3D3DSprite_guess *sprite, unsigned int frameIndex, EQGfxVector3_guess *uvAxis)
{
    T3D3DSpriteFrame_guess *frame;
    T3D3DSpriteDefinition_guess *definition;
    int result;
    int invalidFrame;

    result = 0;
    definition = sprite->definition;
    invalidFrame = frameIndex >= (unsigned int)definition->frame_count
        || (sprite->frames == 0 && EQ_CDECL(int, 0x1000897D, (T3D3DSprite_guess *))(sprite) != 0);
    if (invalidFrame)
    {
        result = 0xffff3d3e;
    }
    else
    {
        frame = sprite->frames;
        frame[frameIndex].u_axis = *uvAxis;
    }
    return result;
}
