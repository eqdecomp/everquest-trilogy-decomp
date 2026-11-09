// unit_10001000.c - eqgfx_dx7.dll, the address run 0x10001000..0x100060EB (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its names are unknown).

#include <string.h>
#include "trilogy/eqaddr.h"

#define EQGFX_COLLISION_COUNTER EQ_GLOBAL(int, 0x10086080)
#define EQGFX_COLLISION_ERROR 0xffff3d3e

typedef struct ClipSegment_guess
{
    int             type;                           // 0x00
    unsigned char   unused_04[0xf8 - 0x04];
    int             collision_counter;               // 0xf8
} ClipSegment_guess;

typedef struct SphereCollisionResult_guess
{
    ClipSegment_guess *segments[2];
    float              fraction;
    float              callback_value;
    unsigned int       collision_data[4];
    unsigned char      unused_20[0x18];
} SphereCollisionResult_guess;

typedef struct MeshVertexStream_guess
{
    unsigned short  vertex_count;          // 0x00
    unsigned char   unused_02[0x0c - 0x02];
    short          *vertices;              // 0x0c  packed xyz shorts for each animated frame
} MeshVertexStream_guess;

// File-local stand-ins for the three objects this function walks. Nothing in include/ describes them, so the
// names and the fields between the ones the original touches are guesses.

typedef struct LigList_guess               // an actor's list of lights (reached from the actor's +0x2c)
{
    int             type;                  // 0x00  0x27 or 0x29 = a list this function edits
    unsigned char   unused_04[0x10 - 0x04];
    unsigned int    light_count;           // 0x10  decremented when the light is removed
    unsigned int    light_capacity;        // 0x14  entries allocated in lights; grown when light_count reaches it
    void          **lights;                // 0x18  light pointers, tail shifted left on a removal
} LigList_guess;

/* Location_guess: now in eqgfx/layout/t3dactor.h. */
#include "eqgfx/layout/t3dactor.h"

typedef struct CollisionVolume_guess
{
    int             type;                  // 0x00  0x2a sphere, 0x2b offset sphere
    union
    {
        struct
        {
            float   unused_04;
            float   radius;                 // 0x08 for type 0x2a
        } sphere;
        struct
        {
            float   offset_x;               // 0x04 for type 0x2b
            float   offset_y;               // 0x08
            float   offset_z;               // 0x0c
            float   radius;                 // 0x10
        } offset_sphere;
    } shape;
} CollisionVolume_guess;

/* T3D_tagACTORINSTANCE: the actor layout is T3D_tagACTORINSTANCE (generated: eqgfx/layout/t3dactor.h); the old local view's members map by offset. */

typedef struct ActorSpriteResource_guess
{
    unsigned char   unused_00[0x0c];
    unsigned int    flags;                  // 0x0c
    unsigned char   unused_10[0x04];
    void           *region_base;            // 0x14
    void           *fallback_region_base;   // 0x18
} ActorSpriteResource_guess;

typedef struct ActorSprite_guess
{
    int             type;                   // 0x00
    unsigned char   unused_04[0x10];
    unsigned char   region_transform[0x10]; // 0x14
    unsigned char   unused_24[0x04];
    unsigned char   alternate_transform[0x0c]; // 0x28
    ActorSpriteResource_guess *resource;    // 0x34
    unsigned char   unused_38[0x24];
    ActorSpriteResource_guess *alternate_resource; // 0x5c
} ActorSprite_guess;

typedef struct World_guess
{
    unsigned char   unused_00[0x24];
    T3D_tagACTORINSTANCE   **actors;                // 0x24  actor pointer array
} World_guess;

typedef struct Light_guess
{
    int             type;                  // 0x00  0x1e or 0x1f = a light that keeps a back-reference list
    unsigned char   unused_04[0x14 - 0x04];
    float           position[3];           // 0x14  the BSP query point the walk starts from
    float           radius_of_influence;   // 0x20  the walk's tolerance
    unsigned int    actor_count;           // 0x24  entries in actor_indices; cleared on the way in
    unsigned int    actor_capacity;        // 0x28  entries allocated in actor_indices; grown when actor_count reaches it
    int            *actor_indices;         // 0x2c  indices into world->actors
} Light_guess;

typedef struct TouchedObject_guess
{
    unsigned char   unused_00[0x28];
    int             own_index;             // 0x28  index into world->actors; called once after the touched list
    unsigned char   unused_2c[0x48 - 0x2c];
    unsigned int    touched_count;         // 0x48  entries in touched; cleared after the walk
    unsigned char   unused_4c[0x50 - 0x4c];
    int            *touched;               // 0x50  indices into world->actors, walked from the last entry down
} TouchedObject_guess;

typedef struct Sphere_guess
{
    unsigned char   unused_00[0x08];
    float           x;                     // 0x08  centre
    float           y;                     // 0x0c
    float           z;                     // 0x10
    float           previous_x;             // 0x14  second point in the sweep
    float           previous_y;             // 0x18
    float           previous_z;             // 0x1c
    float           sweep_x;                 // 0x20  direction used to form the plane intersection point
    float           sweep_y;                 // 0x24
    float           sweep_z;                 // 0x28
    float           dir_x;                 // 0x2c  axis the test runs along (dot product)
    float           dir_y;                 // 0x30
    float           dir_z;                 // 0x34
    float           scale;                 // 0x38  multiplies the signed root term
    float           radius_offset;         // 0x3c  added to the radius argument
    unsigned char   unused_40[0x44 - 0x40];
    float           normal_scale;          // 0x44  scales the clipped normal
    unsigned char   unused_48[0x50 - 0x48];
    float           limit;                 // 0x50  the root term must stay below this
    unsigned char   unused_54[0x5c - 0x54];
    float           normal_x;              // 0x5c  unit vector, written on a return of 1
    float           normal_y;              // 0x60
    float           normal_z;              // 0x64
} Sphere_guess;

typedef struct PolygonVertex_guess
{
    unsigned char   unused_00[0x04];
    float          *point;                 // 0x04  the vertex's x, y, z
    unsigned char   unused_08[0x14 - 0x08];
} PolygonVertex_guess;

typedef struct PolygonSegment_guess
{
    unsigned char   unused_00[0x04];
    float          *start;                  // 0x04  start vertex coordinates
    unsigned char   unused_08[0x0c - 0x08];
    float           parameter_limit;        // 0x0c
    float           direction_scale;        // 0x10  scales the end-minus-start vector
} PolygonSegment_guess;

typedef struct ProjectedPolygon_guess
{
    unsigned char   unused_00[0x04];
    int             vertex_count;          // 0x04  records walked from vertices; the last one closes the loop
    PolygonVertex_guess *vertices;         // 0x08  one record per vertex, 0x14 bytes each
    float           normal_x;              // 0x0c  guess: a normal component, only its sign is read
    float           normal_y;              // 0x10
    float           normal_z;              // 0x14
    float           plane_offset;           // 0x18  added to the plane dot product
    int             projection;            // 0x1c  2 = y/z, 3 = x/z, anything else = x/y
} ProjectedPolygon_guess;

typedef struct BspNode_guess BspNode_guess;

typedef struct BspTree_guess
{
    unsigned char   unused_00[0x0c];
    BspNode_guess  *root;                 // 0x0c
    unsigned char   unused_10[0x14];
    void          **leaf_objects;        // 0x24, one-based leaf indices
    unsigned char   unused_28[0x10];
    unsigned int    pending_capacity;    // 0x38
    BspNode_guess **pending_nodes;       // 0x3c
} BspTree_guess;

struct BspNode_guess
{
    float           plane_x;             // 0x00
    float           plane_y;             // 0x04
    float           plane_z;             // 0x08
    float           plane_offset;        // 0x0c
    BspNode_guess  *front;               // 0x10
    BspNode_guess  *back;                // 0x14
    unsigned int    leaf_index;          // 0x18, nonzero for a leaf
};

#define T3D_SPRITE_REGION_FLAG 0x00400000

// FUNCTION: EQGFX 0x10001000
// Updates the world regions occupied by an actor using its sprite bounds or overall extent.
int __cdecl t3dUpdateTouchedRegions(void *region_context, T3D_tagACTORINSTANCE *actor, void *update_context)
{
    ActorSprite_guess *sprite;
    ActorSpriteResource_guess *resource;
    int sprite_type;
    void *region_data;

    if ((actor->flags_guess[3] & 0x10) == 0)
        goto update_default;

    sprite = EQ_CDECL(ActorSprite_guess *, 0x1000A68E, (T3D_tagACTORINSTANCE *, int, int))(actor, 0, 0);
    sprite_type = sprite->type;
    if (sprite_type == 0x48)
    {
        resource = sprite->resource;
        if ((resource->flags & T3D_SPRITE_REGION_FLAG) != 0)
        {
            if (resource->region_base == 0)
                region_data = (char *)resource->fallback_region_base + 0x5c;
            else
                region_data = (char *)resource->region_base + 0x84;
            return EQ_CDECL(int, 0x10007379, (void *, T3D_tagACTORINSTANCE *, void *, void *, void *))(
                region_context, actor, sprite->region_transform, region_data, update_context);
        }
    }

    if (sprite_type != 0x14)
        goto update_default;

    resource = sprite->alternate_resource;
    if ((resource->flags & T3D_SPRITE_REGION_FLAG) == 0)
        goto update_default;

    region_data = (char *)resource + 0x20;
    return EQ_CDECL(int, 0x10007379, (void *, T3D_tagACTORINSTANCE *, void *, void *, void *))(
        region_context, actor, sprite->alternate_transform, region_data, update_context);

update_default:
    return EQ_CDECL(int, 0x10007118, (void *, T3D_tagACTORINSTANCE *, float, void *))(
        region_context, actor, actor->boundingRadius_guess * actor->scaleFactor_guess, update_context);
}

// FUNCTION: EQGFX 0x10001091
// Removes an actor from each world region that tracks it as touched.
// __cdecl. 0x100070BC per touched entry (last first), then once for own_index; returns the last negative result, else 1.
int __cdecl t3dRemoveObjectFromTouchedRegions(World_guess *world, TouchedObject_guess *object)
{
    T3D_tagACTORINSTANCE **actors;
    int *touched;
    int *cursor;
    int result;
    int call_result;

    actors = world->actors;
    touched = object->touched;
    result = 1;
    if (touched != 0)
    {
        cursor = touched + object->touched_count;
        while (touched < cursor)
        {
            cursor = cursor - 1;
            call_result = EQ_CDECL(int, 0x100070BC, (World_guess *, T3D_tagACTORINSTANCE *, TouchedObject_guess *))
                (world, actors[*cursor], object);
            if (call_result < 0)
                result = call_result;
        }
    }
    object->touched_count = 0;
    call_result = EQ_CDECL(int, 0x100070BC, (World_guess *, T3D_tagACTORINSTANCE *, TouchedObject_guess *))
        (world, actors[object->own_index], object);
    if (call_result < 0)
        result = call_result;
    return result;
}

// FUNCTION: EQGFX 0x1000110a
// Visits world regions within a specified distance of a query point.
int __cdecl traverseBspTree_guess(BspTree_guess *tree, float *query_point, float tolerance,
    int (__cdecl *visit_leaf)(BspTree_guess *, void *, int), int visit_data)
{
    float point_x;
    float point_y;
    float point_z;
    float plane_distance;
    int result;
    unsigned int pending_count;
    unsigned int pending_capacity;
    BspNode_guess *node;
    BspNode_guess **pending_cursor;
    BspNode_guess **new_pending_nodes;

    point_x = query_point[0];
    point_y = query_point[1];
    point_z = query_point[2];
    result = 0;
    pending_count = 0;
    pending_capacity = tree->pending_capacity;
    pending_cursor = tree->pending_nodes;
    node = tree->root;
    do
    {
        if (node != 0)
        {
            do
            {
                if (node->leaf_index != 0)
                    break;
                plane_distance = point_x * node->plane_x + point_y * node->plane_y + point_z * node->plane_z + node->plane_offset;
                if (plane_distance < -tolerance)
                {
                    node = node->back;
                }
                else
                {
                    if (plane_distance <= tolerance && node->back != 0)
                    {
                        if (pending_capacity <= pending_count)
                        {
                            pending_capacity = pending_capacity + 1 + (pending_capacity >> 2);
                            new_pending_nodes = (BspNode_guess **)EQ_CDECL(void *, 0x10050C8C, (void *, unsigned int))
                                (tree->pending_nodes, pending_capacity * 4);
                            if (new_pending_nodes == 0)
                                return -0xc2c2;
                            tree->pending_nodes = new_pending_nodes;
                            tree->pending_capacity = pending_capacity;
                            pending_cursor = tree->pending_nodes + pending_count;
                        }
                        *pending_cursor = node->back;
                        pending_cursor++;
                        pending_count++;
                    }
                    node = node->front;
                }
            } while (node != 0);
            if (node != 0)
            {
                result = visit_leaf(tree, tree->leaf_objects[node->leaf_index - 1], visit_data);
                if (result != 0)
                    return result;
            }
        }
        if (pending_count == 0)
            return result;
        pending_cursor--;
        pending_count--;
        node = *pending_cursor;
    } while (1);
}

// The shift below is the original's inline rep movsd, which needs the memcpy intrinsic (scoped to this function).
#pragma intrinsic(memcpy)

// FUNCTION: EQGFX 0x1000121b
// Clears a light's recorded influence on previously affected actors.
// __cdecl (two stack arguments, callee pops nothing; ECX is only scratch). Returns 0 on every path.
// Removes `light` from the light lists of every actor in the light's back-reference list. Only the two light
// types with such a list (0x1e/0x1f) do anything: the list (actor_indices[0..actor_count), count cleared
// first) is walked, each index picks world->actors[i], and that actor's liglist at +0x2c is edited only when
// its type is 0x27 or 0x29 -- the first matching pointer in liglist->lights comes out, the tail shifts left
// and light_count drops. The bounds are compared unsigned, so an end below the array walks nothing.
int __cdecl _t3dRemoveFromLitRegions_guess(World_guess *world, int *light)
{
    Light_guess *searched;      // EBX: the light the liglist scan compares against
    int *actor_index;           // EDX: walks the light's back-reference list
    unsigned int count;
    LigList_guess *liglist;
    void **light_slot;
    void **light_slot_end;

    searched = (Light_guess *)light;
    if (searched->type == 0x1e || searched->type == 0x1f)
    {
        actor_index = searched->actor_indices;
        count = searched->actor_count;
        searched->actor_count = 0;
        light = actor_index + count;        // the parameter becomes the end of the walk
    }
    else
    {
        actor_index = light;
    }
    while (actor_index < light)
    {
        liglist = ((T3D_tagACTORINSTANCE *)world->actors[*actor_index])->ligList_guess;
        if (liglist->type == 0x27 || liglist->type == 0x29)
        {
            light_slot = liglist->lights;
            light_slot_end = light_slot + liglist->light_count;
            while (light_slot < light_slot_end)
            {
                if (*light_slot == (void *)searched)
                {
                    if (light_slot < light_slot_end - 1)
                        memcpy(light_slot, light_slot + 1,
                               (light_slot_end - 1 - light_slot) * sizeof(void *));
                    liglist->light_count--;
                    break;
                }
                light_slot++;
            }
        }
        actor_index++;
    }
    return 0;
}

// File-local stand-ins for the transform, the origin object and the sprite this function walks. Nothing in
// include/ describes them, so the names and the fields between the ones the original touches are guesses.

typedef struct Transform_guess
{
    unsigned char unused_00[0x10];
    float         position[3];      // 0x10
    unsigned char unused_1c[0x34 - 0x1c];
    float         field_34;         // 0x34
    float         field_38;         // 0x38
    unsigned char unused_3c[0x54 - 0x3c];
    float         field_54;         // 0x54
    unsigned char unused_58[0x5c - 0x58];
    float         matrix[3][3];     // 0x5c  input axis c at 0x5c + 12*c; out = point * matrix + offset
    float         offset[3];        // 0x80
    float         basis[3][3];      // 0x8c  row r at 0x8c + 12*r; out = vertex * basis + translation
    float         translation[3];   // 0xb0
    float         direction_basis[3][3];  // 0xbc  row r at 0xbc + 12*r; out = direction_basis * vector, no translation
} Transform_guess;

typedef struct Origin_guess
{
    unsigned char unused_00[0x14];
    float         x;                // 0x14
    float         y;                // 0x18
    float         z;                // 0x1c
    unsigned char unused_20[0x34 - 0x20];
    float         field_34;         // 0x34
} Origin_guess;

typedef struct Sprite_guess
{
    float         vertex[9];        // 0x00  in: two xyz vertices; out: both transformed plus their difference
    unsigned char unused_24[0x34 - 0x24];
    int           field_34;         // 0x34  copied straight through
    int           field_38;         // 0x38
    unsigned char unused_3c[0x40 - 0x3c];
    int           field_40;         // 0x40
    unsigned char unused_44[0x48 - 0x44];
    int           field_48;         // 0x48
    int           field_4c;         // 0x4c  overwritten with the third argument
    int           field_50;         // 0x50
    int           field_54;         // 0x54
    int           field_58;         // 0x58
    int           field_5c;         // 0x5c
} Sprite_guess;

// File-local stand-in for the transform this function reads. Nothing in include/ describes it, so the name
// and the fields between the ones the original touches are guesses. position is subtracted from each source
// vertex, then the difference is multiplied by the basis at 0xb0, element [c][r] at 0xb0 + 12*c + 4*r
// (c = source axis, r = output axis).

typedef struct PointTransform_guess
{
    unsigned char unused_00[0x10];
    float         position[3];      // 0x10  subtracted from the source vertex
    unsigned char unused_1c[0x34 - 0x1c];
    float         field_34;         // 0x34  radius out = radius in / field_34; field_38 out = field_38 in * field_34
    unsigned char unused_38[0x68 - 0x38];
    float         direction_basis[3][3];  // 0x68  row r at 0x68 + 12*r; out = direction_basis * vector, no translation
    unsigned char unused_8c[0xb0 - 0x8c];
    float         basis[3][3];      // 0xb0  out axis r = basis[0][r] * dx + basis[1][r] * dy + basis[2][r] * dz
} PointTransform_guess;

// File-local stand-in for the beam s3dTransformMovingSphereByMatrix_guess reads and writes. Its vertices sit at +0x08; Sprite_guess has them at
// +0x00, so this is a separate layout. The meanings of the fields between the touched ones are guesses.

typedef struct Beam_guess
{
    int             field_00;         // 0x00  copied straight through
    int             field_04;         // 0x04  copied straight through
    float           start[3];         // 0x08  in: first vertex; out: it transformed, minus the origin
    float           end[3];           // 0x14  in: second vertex; out: it transformed, minus the origin
    float           delta[3];         // 0x20  out: end - start
    float           dir_a[3];         // 0x2c  in: a direction; out: direction_basis * dir_a
    float           field_38;         // 0x38  out: transform field_38 * in field_38 * origin field_34
    float           radius;           // 0x3c  in: a length; out: scaled by transform field_54 and origin field_34
    float           radius_sq;        // 0x40  out: radius squared
    float           radius_inv;       // 0x44  out: 1.0 / radius
    int             field_48;         // 0x48  copied straight through
    int             field_4c;         // 0x4c  copied straight through
    int             field_50;         // 0x50  copied straight through
    unsigned char   unused_54[0x5c - 0x54];
    float           dir_b[3];         // 0x5c  in: a direction; out: direction_basis * dir_b
} Beam_guess;

typedef struct SpriteCollisionEntry_guess
{
    unsigned char unused_00[0x08];
    void          *sprite;            // 0x08
    unsigned char unused_0c[0x38 - 0x0c];
    float          radius;            // 0x38
    unsigned char unused_3c[0x148 - 0x3c];
} SpriteCollisionEntry_guess;

typedef struct SpriteCollisionList_guess
{
    unsigned char             unused_00[0x20];
    int                       count;  // 0x20
    SpriteCollisionEntry_guess *entries; // 0x24
} SpriteCollisionList_guess;

typedef struct SpriteCollisionVolume_guess
{
    int                       type;   // 0x00
    float                     parameters[5]; // 0x04
    struct SpriteCollisionData_guess *data; // 0x18
} SpriteCollisionVolume_guess;

typedef struct SpriteCollisionData_guess
{
    unsigned char unused_00[0x14];
    float         radius;             // 0x14
    int           polygon_count;      // 0x18
    void         *polygons;           // 0x1c
} SpriteCollisionData_guess;

typedef struct SpriteCollisionState_guess
{
    void                     *actor_registry; // 0x00
    int                       reserved_argument;
    float                     segment_start[3]; // 0x08
    float                     segment_end[3]; // 0x14
    float                     delta[3];
    float                     direction[3];
    float                     inverse_length;
    float                     radius;
    float                     radius_squared;
    float                     inverse_radius;
    int (__cdecl *actor_filter)(void *, int);
    int                       filter_context;
    float                     fraction;
    void                     *hit_actor;
    SpriteCollisionEntry_guess *last_hit;
} SpriteCollisionState_guess;

typedef struct ActorRegistry_guess
{
    unsigned char unused_00[0x17c];
    T3D_tagACTORINSTANCE **actors_by_index;          // 0x17c
} ActorRegistry_guess;

typedef struct ActorIndexList_guess
{
    unsigned char unused_00[0x04];
    unsigned int  count;                    // 0x04
    unsigned char unused_08[0x0c - 0x08];
    unsigned int *indices;                  // 0x0c
} ActorIndexList_guess;

typedef struct CollisionScene_guess
{
    unsigned char        unused_00[0x24];
    void                *collision_source; // 0x24
    unsigned char        unused_28[0x2c - 0x28];
    ActorIndexList_guess *active_actors;   // 0x2c
} CollisionScene_guess;

typedef struct CollisionPlane_guess
{
    float normal_x;
    float normal_y;
    float normal_z;
    float distance;
    struct CollisionPlane_guess *front;
    struct CollisionPlane_guess *back;
    int scene_index;
} CollisionPlane_guess;

typedef struct CollisionTraversalEntry_guess
{
    CollisionPlane_guess *plane;
    int                   side;
    int                   unused_08[4];
} CollisionTraversalEntry_guess;

typedef struct CollisionWorld_guess
{
    unsigned char          unused_00[0x0c];
    CollisionPlane_guess *root;
    unsigned char          unused_10[0x14];
    CollisionScene_guess **scenes_by_index;
    unsigned char          unused_28[0x35c - 0x28];
    CollisionTraversalEntry_guess *traversal_stack;
} CollisionWorld_guess;

typedef struct MovingSphereResult_guess
{
    void                      *hit_actor;
    SpriteCollisionEntry_guess *hit_entry;
    float                      fraction;
    float                      distance;
    float                      normal_x;
    float                      normal_y;
    float                      normal_z;
    float                      penetration;
    float                      offset_x;
    float                      offset_y;
    float                      offset_z;
} MovingSphereResult_guess;

typedef float (__cdecl *MovingSphereInvSqrt_guess)(float squared_length);

typedef struct CollisionSource_guess
{
    unsigned char unused_00[0x1c];
    void          *final_object;           // 0x1c
} CollisionSource_guess;

typedef struct CollisionModel_guess
{
    unsigned char unused_00[0x14];
    void          *animated_data;          // 0x14
    void          *static_data;            // 0x18
} CollisionModel_guess;

typedef struct AnimatedCollisionData_guess
{
    unsigned char unused_00[0x20];
    unsigned int  part_count;               // 0x20
    unsigned char unused_24[0x30 - 0x24];
    float         radius;                   // 0x30
} AnimatedCollisionData_guess;

typedef struct StaticCollisionData_guess
{
    unsigned char unused_00[0x08];
    unsigned short polygon_count;           // 0x08
    unsigned char unused_0a[0x2c - 0x0a];
    float         origin[3];                // 0x2c
    unsigned char unused_38[0x58 - 0x38];
    float         radius;                   // 0x58
} StaticCollisionData_guess;

typedef struct HierarchicalSpriteObject_guess
{
    int           type;                     // 0x00
    unsigned char unused_04[0x34 - 0x04];
    CollisionModel_guess *collision_model;  // 0x34
} HierarchicalSpriteObject_guess;

#define EQGFX_COLLISION_COMPLETE_GUESS EQ_GLOBAL(int, 0x100AAD08)
#define EQGFX_COLLISION_BUSY_GUESS     EQ_GLOBAL(int, 0x10086080)
#define EQGFX_COLLISION_OUTPUT_GUESS   EQ_ARRAY(unsigned char, 0x100AACA0)
#define EQGFX_COLLISION_DEFAULT_GUESS  EQ_ARRAY(unsigned char, 0x100AAD70)
#define EQGFX_GET_ACTOR_SPRITE_GUESS EQ_CDECL(ActorSprite_guess *, 0x1000A68E, (T3D_tagACTORINSTANCE *, unsigned int, int))
#define EQGFX_GET_SPRITE_CENTER_GUESS EQ_CDECL(void, 0x1001B263, (void *, float *))
#define EQGFX_GET_SPRITE_VOLUME_GUESS EQ_CDECL(void, 0x1001B20B, (void *, SpriteCollisionVolume_guess **))
#define EQGFX_TEST_SPHERE_GUESS EQ_CDECL(int, 0x1000306A, (SpriteCollisionState_guess *, float *, float))
#define EQGFX_UPDATE_ACTOR_TRANSFORM_GUESS EQ_CDECL(void, 0x10035058, (T3D_tagACTORINSTANCE *, float, float *))
#define EQGFX_BUILD_COLLISION_ORIGIN_GUESS EQ_CDECL(void, 0x1000270A, (T3D_tagACTORINSTANCE *, float *, SpriteCollisionState_guess *, void *))
#define EQGFX_TEST_HIERARCHICAL_COLLISION_GUESS EQ_CDECL(int, 0x10006BC9, (void *, void *))
#define EQGFX_TEST_ANIMATED_PART_GUESS EQ_CDECL(int, 0x100039F7, (void *, void *, unsigned int))
#define EQGFX_TEST_STATIC_POLYGON_GUESS EQ_CDECL(int, 0x10003CA3, (void *, void *, unsigned int))
#define EQGFX_TEST_POLYGON_GUESS EQ_CDECL(int, 0x1000312F, (void *, void *))
#define EQGFX_GET_ACTOR_CENTER_GUESS EQ_CDECL(void, 0x100046ED, (T3D_tagACTORINSTANCE *, float *, float *))
#define EQGFX_UPDATE_HIERARCHICAL_TRANSFORMS_GUESS EQ_CDECL(void, 0x100327C3, (void *, int, T3D_tagACTORINSTANCE *))
#define EQGFX_TEST_SPRITE_COLLISION_GUESS EQ_CDECL(int, 0x100025D6, (SpriteCollisionState_guess *, T3D_tagACTORINSTANCE *, void *))
#define EQGFX_TEST_SEGMENT_SPHERE_GUESS EQ_CDECL(int, 0x10002F16, (SpriteCollisionState_guess *, float *, float))
#define EQGFX_TEST_TRANSFORMED_SPRITE_GUESS EQ_CDECL(int, 0x10006D73, (void *, void *))
#define EQGFX_FINISH_COLLISION_GUESS EQ_CDECL(void, 0x10002E32, (T3D_tagACTORINSTANCE *, SpriteCollisionState_guess *, void *))

typedef int (__cdecl *LeafVisitor_guess)(BspTree_guess *tree, void *leaf_object, int visit_data);

// FUNCTION: EQGFX 0x1000129D
// Updates the actors linked to a light within its influence radius.
// __cdecl. Calls _t3dRemoveFromLitRegions_guess, then walks the BSP tree from the light's position within its radius, visiting with the light.
int __cdecl traverseLightInfluenceBsp_guess(World_guess *world, Light_guess *light)
{
    EQ_CDECL(int, 0x1000121B, (World_guess *, int *))(world, (int *)light);
    return EQ_CDECL(int, 0x1000110A, (BspTree_guess *, float *, float, LeafVisitor_guess, int))
        ((BspTree_guess *)world, light->position, light->radius_of_influence, (LeafVisitor_guess)0x100012cd, (int)light);
}

// FUNCTION: EQGFX 0x100012CD
// Registers a light's influence on one actor and records that actor for later removal.
// __cdecl leaf visitor: appends the light to the actor's liglist when its type is 0x27 or 0x29, then the actor's world index to the light's actor_indices; a full array grows first. Returns 0, or 0xffff3d3e when a grow fails.
int __cdecl _t3dAddLightToActor_guess(BspTree_guess *tree, T3D_tagACTORINSTANCE *actor, Light_guess *light)
{
    int result;
    LigList_guess *liglist;
    unsigned int new_capacity;
    void **new_lights;
    int *new_indices;

    result = 0;
    liglist = actor->ligList_guess;
    if (liglist->type != 0x27 && liglist->type != 0x29)
        return result;
    if (liglist->light_count >= liglist->light_capacity)
    {
        new_capacity = (liglist->light_capacity >> 2) + liglist->light_capacity + 1;
        new_lights = (void **)EQ_CDECL(void *, 0x10050C8C, (void *, unsigned int))(liglist->lights, new_capacity * 4);
        if (new_lights == 0)
        {
            result = 0xffff3d3e;
            return result;
        }
        liglist->light_capacity = new_capacity;
        liglist->lights = new_lights;
    }
    liglist->lights[liglist->light_count] = light;
    liglist->light_count++;
    if (light->actor_count >= light->actor_capacity)
    {
        new_capacity = (light->actor_capacity >> 2) + light->actor_capacity + 1;
        new_indices = (int *)EQ_CDECL(void *, 0x10050C8C, (void *, unsigned int))(light->actor_indices, new_capacity * 4);
        if (new_indices == 0)
        {
            result = 0xffff3d3e;
            return result;
        }
        light->actor_capacity = new_capacity;
        light->actor_indices = new_indices;
    }
    light->actor_indices[light->actor_count] = actor->listIndex_guess;
    light->actor_count++;
    return result;
}

// FUNCTION: EQGFX 0x1000136E
// Moves an actor toward a requested position, stopping its collision sphere at world obstacles.
int __cdecl s3dCollideActorWithWorld(T3D_tagACTORINSTANCE *actor, void *world, int collision_context,
    float *requested_position, int collision_mask,
    int (__cdecl *segment_callback)(ClipSegment_guess *, void *, float), void *callback_context,
    float *hit_position, unsigned int *collision_data, unsigned int *collision_id,
    int include_actor)
{
    int result;
    CollisionVolume_guess *collision_volume;
    char message[256];
    float saved_position[7];
    float offset_position[3];
    float collision_start[3];
    int actor_filter;
    float sphere_radius;
    int set_location_result;

    result = 0;
    if (actor == 0 || (collision_volume = actor->collisionShape_guess) == 0)
    {
        result = EQGFX_COLLISION_ERROR;
    }
    else
    {
        memcpy(saved_position, actor->location_guess.value, sizeof(saved_position));
        actor_filter = 0;
        if (include_actor == 1)
            actor_filter = (int)actor;
        if (collision_volume->type == 0x2a)
        {
            collision_start[0] = saved_position[0];
            collision_start[1] = saved_position[1];
            collision_start[2] = saved_position[2];
            sphere_radius = collision_volume->shape.sphere.radius * actor->scaleFactor_guess;
            EQ_CDECL(void, 0x100016DC,
                (void *, int, float *, float *, float, int,
                 int (__cdecl *)(ClipSegment_guess *, void *, float), void *, float *,
                 unsigned int *, unsigned int *, int))
                (world, collision_context, collision_start, requested_position, sphere_radius,
                 collision_mask, segment_callback, callback_context, hit_position,
                 collision_data, collision_id, actor_filter);
        }
        else
        {
            if (collision_volume->type != 0x2b)
            {
                EQ_CDECL(int, 0x10067992, (char *, const char *, ...))(
                    message, EQ_ARRAY(const char, 0x10086084),
                    collision_volume->type, 0x2a, 0x2b);
                return EQGFX_COLLISION_ERROR;
            }
            collision_start[0] = saved_position[0];
            collision_start[1] = saved_position[1];
            collision_start[2] = saved_position[2];
            offset_position[0] = actor->scaleFactor_guess * collision_volume->shape.offset_sphere.offset_x +
                requested_position[0];
            offset_position[1] = collision_volume->shape.offset_sphere.offset_y * actor->scaleFactor_guess +
                requested_position[1];
            offset_position[2] = collision_volume->shape.offset_sphere.offset_z * actor->scaleFactor_guess +
                requested_position[2];
            sphere_radius = collision_volume->shape.offset_sphere.radius * actor->scaleFactor_guess;
            EQ_CDECL(void, 0x100016DC,
                (void *, int, float *, float *, float, int,
                 int (__cdecl *)(ClipSegment_guess *, void *, float), void *, float *,
                 unsigned int *, unsigned int *, int))
                (world, collision_context, collision_start, offset_position, sphere_radius,
                 collision_mask, segment_callback, callback_context, hit_position,
                 collision_data, collision_id, actor_filter);
            hit_position[0] = hit_position[0] - actor->scaleFactor_guess *
                collision_volume->shape.offset_sphere.offset_x;
            hit_position[1] = hit_position[1] - collision_volume->shape.offset_sphere.offset_y *
                actor->scaleFactor_guess;
            hit_position[2] = hit_position[2] - collision_volume->shape.offset_sphere.offset_z *
                actor->scaleFactor_guess;
        }
        saved_position[0] = hit_position[0];
        saved_position[1] = hit_position[1];
        saved_position[2] = hit_position[2];
        set_location_result = EQ_CDECL(int, 0x1000A764, (void *, T3D_tagACTORINSTANCE *, float *))
            (world, actor, saved_position);
        if (set_location_result != 0)
        {
            saved_position[0] = collision_start[0];
            saved_position[1] = collision_start[1];
            saved_position[2] = collision_start[2];
            EQ_CDECL(int, 0x1000A764, (void *, T3D_tagACTORINSTANCE *, float *))
                (world, actor, saved_position);
            hit_position[0] = collision_start[0];
            hit_position[1] = collision_start[1];
            hit_position[2] = collision_start[2];
            result = EQGFX_COLLISION_ERROR;
            *collision_id = 0;
            collision_data[0] = 0;
            collision_data[1] = 0;
            collision_data[2] = 0;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1000151D
// Moves an actor a specified distance along a direction, stopping at world obstacles.
int __cdecl s3dCollideActorWithWorldAlongUnitDirection(T3D_tagACTORINSTANCE *actor, void *world,
    int collision_context, float *direction, float distance, int collision_mask,
    int (__cdecl *segment_callback)(ClipSegment_guess *, void *, float), void *callback_context,
    float *hit_position, unsigned int *collision_data, unsigned int *collision_id)
{
    int result;
    CollisionVolume_guess *collision_volume;
    float saved_position[7];
    float collision_start[3];
    float end_position[3];
    float actor_start[3];
    float *source_position;
    float *copy_destination;
    int copy_count;
    int set_location_result;

    collision_volume = actor->collisionShape_guess;
    source_position = &actor->location_guess.value[0];
    copy_destination = saved_position;
    for (copy_count = 7; copy_count != 0; copy_count--)
    {
        *copy_destination = *source_position;
        source_position++;
        copy_destination++;
    }
    result = 0;
    if (collision_volume->type == 0x2a)
    {
        actor_start[0] = saved_position[0];
        end_position[0] = distance * direction[0] + saved_position[0];
        actor_start[1] = saved_position[1];
        end_position[1] = distance * direction[1] + saved_position[1];
        actor_start[2] = saved_position[2];
        end_position[2] = distance * direction[2] + saved_position[2];
        EQ_CDECL(void, 0x100016DC, (void *, int, float *, float *, float, int,
            int (__cdecl *)(ClipSegment_guess *, void *, float), void *, float *,
            unsigned int *, unsigned int *, T3D_tagACTORINSTANCE *))(world, collision_context,
            actor_start, end_position,
            collision_volume->shape.sphere.radius * actor->scaleFactor_guess,
            collision_mask, segment_callback, callback_context, hit_position,
            collision_data, collision_id, 0);
    }
    else
    {
        if (collision_volume->type != 0x2b)
        {
            result = EQGFX_COLLISION_ERROR;
            return result;
        }
        actor_start[0] = saved_position[0];
        actor_start[1] = saved_position[1];
        actor_start[2] = saved_position[2];
        collision_start[0] = actor->scaleFactor_guess * collision_volume->shape.offset_sphere.offset_x + saved_position[0];
        collision_start[1] = actor->scaleFactor_guess * collision_volume->shape.offset_sphere.offset_y + saved_position[1];
        collision_start[2] = collision_volume->shape.offset_sphere.offset_z * actor->scaleFactor_guess + saved_position[2];
        end_position[0] = distance * direction[0] + collision_start[0];
        end_position[1] = distance * direction[1] + collision_start[1];
        end_position[2] = distance * direction[2] + collision_start[2];
        EQ_CDECL(void, 0x100016DC, (void *, int, float *, float *, float, int,
            int (__cdecl *)(ClipSegment_guess *, void *, float), void *, float *,
            unsigned int *, unsigned int *, T3D_tagACTORINSTANCE *))(world, collision_context,
            collision_start, end_position,
            collision_volume->shape.offset_sphere.radius * actor->scaleFactor_guess,
            collision_mask, segment_callback, callback_context, hit_position,
            collision_data, collision_id, 0);
        hit_position[0] = hit_position[0]
            - actor->scaleFactor_guess * collision_volume->shape.offset_sphere.offset_x;
        hit_position[1] = hit_position[1]
            - actor->scaleFactor_guess * collision_volume->shape.offset_sphere.offset_y;
        hit_position[2] = hit_position[2]
            - collision_volume->shape.offset_sphere.offset_z * actor->scaleFactor_guess;
    }
    saved_position[0] = hit_position[0];
    saved_position[1] = hit_position[1];
    saved_position[2] = hit_position[2];
    set_location_result = EQ_CDECL(int, 0x1000A764, (void *, T3D_tagACTORINSTANCE *, float *))(
        world, actor, saved_position);
    if (set_location_result != 0)
    {
        saved_position[0] = actor_start[0];
        saved_position[1] = actor_start[1];
        saved_position[2] = actor_start[2];
        EQ_CDECL(int, 0x1000A764, (void *, T3D_tagACTORINSTANCE *, float *))(
            world, actor, saved_position);
        hit_position[0] = actor_start[0];
        hit_position[1] = actor_start[1];
        hit_position[2] = actor_start[2];
        result = EQGFX_COLLISION_ERROR;
        collision_data[0] = 0;
        collision_data[1] = 0;
        collision_data[2] = 0;
        *collision_id = 0;
    }
    return result;
}

// FUNCTION: EQGFX 0x100016DC
// Finds where a sphere's movement stops in the world, allowing callbacks to pass through selected contacts.
int __cdecl s3dCollideSphereWithWorld(void *world, int collision_context,
    float *start_position, float *end_position, float radius, int collision_mask,
    int (__cdecl *segment_callback)(ClipSegment_guess *, void *, float), void *callback_context,
    float *hit_position, unsigned int *collision_data, unsigned int *collision_id,
    T3D_tagACTORINSTANCE *orientation_actor)
{
    SphereCollisionResult_guess collision_result;
    ClipSegment_guess *segment;
    float delta_y;
    float delta_z;
    int reallocate_result;

    segment = 0;
    reallocate_result = EQ_CDECL(int, 0x10006E31, (void *))(world);
    if (reallocate_result != 0)
        return -1;

    EQGFX_COLLISION_COUNTER = EQGFX_COLLISION_COUNTER + 1;
    EQGFX_COLLISION_COMPLETE_GUESS = EQGFX_COLLISION_COUNTER;
    do
    {
        EQGFX_COLLISION_COUNTER = EQGFX_COLLISION_COUNTER + 1;
        if (segment != 0)
        {
            if (segment->type != 0x18)
            {
                segment_callback(segment, callback_context, collision_result.callback_value);
                break;
            }
            if (segment_callback == 0
                || segment_callback(segment, callback_context, collision_result.callback_value) == 1)
                break;
            segment->collision_counter = EQGFX_COLLISION_COMPLETE_GUESS;
        }
        EQ_CDECL(void, 0x10001A67,
            (void *, int, float *, float *, float, int, void *, SphereCollisionResult_guess *))
            (world, collision_context, start_position, end_position, radius, collision_mask,
                callback_context, &collision_result);
        segment = collision_result.segments[0];
    } while (segment != 0);

    if (orientation_actor != 0)
        EQ_CDECL(int, 0x100017EF, (void *, T3D_tagACTORINSTANCE *, float *))
            (world, orientation_actor, (float *)&collision_result);

    delta_y = end_position[1] - start_position[1];
    delta_z = end_position[2] - start_position[2];
    hit_position[0] = collision_result.fraction * (end_position[0] - start_position[0]) + start_position[0];
    hit_position[1] = collision_result.fraction * delta_y + start_position[1];
    hit_position[2] = collision_result.fraction * delta_z + start_position[2];
    *collision_id = collision_result.collision_data[3];
    collision_data[0] = collision_result.collision_data[0];
    collision_data[1] = collision_result.collision_data[1];
    collision_data[2] = collision_result.collision_data[2];
    return 0;
}

// FUNCTION: EQGFX 0x100017EF
// Adjusts an actor's pitch and roll to its collision surface according to its orientation mode.
// __cdecl: sets the actor's location with its pitch and roll from the coefficients (orientation_mode 1, or a first coefficient whose raw bits are zero, levels both; 2 sets pitch; 3 sets pitch and roll). Returns 1, 0 for mode 0, -1 for another mode.
int __cdecl s3dReorientActorFromCollision(void *world, T3D_tagACTORINSTANCE *actor, float *coefficients)
{
    T3D_tagACTORINSTANCE *target;
    int mode;
    Location_guess location;
    float coeff_x;
    float coeff_y;
    float coeff_z;
    float cosine;
    float sine;
    float dir_x;
    float dir_y;
    float dir_z;
    float length_squared;
    float inverse_length;
    float pitch_sine;

    target = actor;
    mode = target->surfacePitchType_int_guess;
    if (mode == 0)
        return 0;
    location = target->location_guess;
    if (*(int *)coefficients == 0 || mode == 1)
    {
        location.value[4] = 0.0f;
        location.value[5] = 0.0f;
    }
    else if (mode == 2)
    {
        coeff_x = coefficients[8];
        coeff_y = coefficients[9];
        coeff_z = coefficients[10];
        cosine = EQ_CDECL(float, 0x1004EEFA, (float))(location.value[3]);
        sine = EQ_CDECL(float, 0x1004EE87, (float))(location.value[3]);
        dir_y = -(sine * coeff_z);
        dir_z = cosine * coeff_z;
        dir_x = sine * coeff_x - cosine * coeff_y;
        length_squared = dir_x * dir_x + dir_z * dir_z + dir_y * dir_y;
        if (length_squared != 0.0f)
        {
            inverse_length = EQ_GLOBAL(MovingSphereInvSqrt_guess, 0x100FC77C)(length_squared);
            dir_y = inverse_length * dir_y;
            dir_z = inverse_length * dir_z;
        }
        pitch_sine = dir_y * coeff_y - dir_z * coeff_x;
        if (pitch_sine == 0.0f)
            location.value[4] = 0.0f;
        else
            location.value[4] = EQ_CDECL(float, 0x1004ECF9, (float, float, void *))(pitch_sine, EQ_CDECL(float, 0x1004EF68, (float))(1.0f - pitch_sine * pitch_sine), &actor);
        location.value[5] = 0.0f;
    }
    else if (mode == 3)
    {
        coeff_x = coefficients[8];
        coeff_y = coefficients[9];
        coeff_z = coefficients[10];
        cosine = EQ_CDECL(float, 0x1004EEFA, (float))(location.value[3]);
        sine = EQ_CDECL(float, 0x1004EE87, (float))(location.value[3]);
        dir_y = -(sine * coeff_z);
        dir_z = cosine * coeff_z;
        dir_x = sine * coeff_x - cosine * coeff_y;
        length_squared = dir_x * dir_x + dir_z * dir_z + dir_y * dir_y;
        if (length_squared != 0.0f)
        {
            inverse_length = EQ_GLOBAL(MovingSphereInvSqrt_guess, 0x100FC77C)(length_squared);
            dir_y = inverse_length * dir_y;
            dir_z = inverse_length * dir_z;
            dir_x = inverse_length * dir_x;
        }
        pitch_sine = dir_y * coeff_y - dir_z * coeff_x;
        if (pitch_sine == 0.0f)
            location.value[4] = 0.0f;
        else
            location.value[4] = EQ_CDECL(float, 0x1004ECF9, (float, float, void *))(pitch_sine, EQ_CDECL(float, 0x1004EF68, (float))(1.0f - pitch_sine * pitch_sine), &actor);
        if (dir_x == 0.0f)
            location.value[5] = 0.0f;
        else
            location.value[5] = EQ_CDECL(float, 0x1004ECF9, (float, float, void *))(dir_x, EQ_CDECL(float, 0x1004EF68, (float))(1.0f - dir_x * dir_x), &actor);
    }
    else
    {
        return -1;
    }
    EQ_CDECL(void, 0x1000A764, (void *, T3D_tagACTORINSTANCE *, Location_guess *))(world, target, &location);
    return 1;
}

// FUNCTION: EQGFX 0x10001a67
// Sweeps a sphere from start to end through the world BSP; reports the nearest hit, its fraction and normal
int __cdecl s3dCollideMovingSphereWithWorld(CollisionWorld_guess *world, int reserved_argument, float *start,
                         float *end, float radius, int (__cdecl *actor_filter)(void *, int),
                         int filter_context, MovingSphereResult_guess *collision_result)
{
    SpriteCollisionState_guess collision_state;
    CollisionPlane_guess *plane;
    CollisionScene_guess *scene;
    CollisionTraversalEntry_guess *candidate_cursor;
    float start_x;
    float start_y;
    float start_z;
    float end_x;
    float end_y;
    float end_z;
    float current_end_x;
    float current_end_y;
    float current_end_z;
    float delta_x;
    float delta_y;
    float delta_z;
    float direction_x;
    float direction_y;
    float direction_z;
    float distance_squared;
    float segment_length;
    float inverse_length;
    float radius_squared;
    float inverse_radius;
    float start_plane_distance;
    float end_plane_distance;
    float projection;
    float normal_x;
    float normal_y;
    float normal_z;
    float normal_squared;
    float inverse_normal_length;
    float penetration;
    float offset_x;
    float offset_y;
    float offset_z;
    int leaf_index;
    int candidate_side;

    if (radius == EQ_GLOBAL(float, 0x10076284))
    {
        EQ_CDECL(void, 0x100509EE, (const char *message))(
            "ERROR: fRadius is zero s3dCollideMovingSphereWithWorld!\n");
        return -1;
    }
    start_x = start[0];
    start_y = start[1];
    start_z = start[2];
    end_x = end[0];
    end_y = end[1];
    end_z = end[2];
    collision_state.fraction = 1.0f;
    collision_state.hit_actor = 0;
    collision_state.last_hit = 0;
    normal_x = 0.0f;
    normal_y = 0.0f;
    normal_z = 0.0f;
    penetration = 0.0f;
    offset_x = 0.0f;
    offset_y = 0.0f;
    offset_z = 0.0f;
    delta_x = end_x - start_x;
    delta_y = end_y - start_y;
    delta_z = end_z - start_z;
    distance_squared = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
    if (distance_squared == EQ_GLOBAL(float, 0x10076284))
    {
        collision_result->hit_actor = 0;
        collision_result->hit_entry = 0;
        collision_result->fraction = 1.0f;
        collision_result->distance = 0.0f;
        collision_result->normal_x = 0.0f;
        collision_result->normal_y = 0.0f;
        collision_result->normal_z = 0.0f;
        collision_result->penetration = 0.0f;
        collision_result->offset_x = 0.0f;
        collision_result->offset_y = 0.0f;
        collision_result->offset_z = 0.0f;
        return 0;
    }
    segment_length = EQ_CDECL(float, 0x1004EF68, (float squared_length))(distance_squared);
    inverse_length = 1.0f / segment_length;
    radius_squared = radius * radius;
    inverse_radius = 1.0f / radius;
    direction_x = inverse_length * delta_x;
    direction_y = inverse_length * delta_y;
    direction_z = inverse_length * delta_z;
    collision_state.actor_registry = world;
    collision_state.reserved_argument = reserved_argument;
    collision_state.segment_start[0] = start_x;
    collision_state.segment_start[1] = start_y;
    collision_state.segment_start[2] = start_z;
    collision_state.segment_end[0] = end_x;
    collision_state.segment_end[1] = end_y;
    collision_state.segment_end[2] = end_z;
    collision_state.delta[0] = delta_x;
    collision_state.delta[1] = delta_y;
    collision_state.delta[2] = delta_z;
    collision_state.direction[0] = direction_x;
    collision_state.direction[1] = direction_y;
    collision_state.direction[2] = direction_z;
    collision_state.inverse_length = inverse_length;
    collision_state.radius = radius;
    collision_state.radius_squared = radius_squared;
    collision_state.inverse_radius = inverse_radius;
    collision_state.actor_filter = actor_filter;
    collision_state.filter_context = filter_context;
    current_end_x = end_x;
    current_end_y = end_y;
    current_end_z = end_z;
    plane = world->root;
    candidate_cursor = world->traversal_stack - 1;
walk_tree:
    do
    {
        if (plane != 0)
        {
            if (plane->scene_index == 0)
            {
                start_plane_distance = start_y * plane->normal_y + start_z * plane->normal_z +
                                       start_x * plane->normal_x + plane->distance;
                end_plane_distance = current_end_y * plane->normal_y + current_end_z * plane->normal_z +
                                    current_end_x * plane->normal_x + plane->distance;
                if (start_plane_distance < radius || end_plane_distance < radius)
                {
                    if (-radius < start_plane_distance || -radius < end_plane_distance)
                    {
                        if (EQ_GLOBAL(float, 0x10076284) <= start_plane_distance)
                        {
                            candidate_cursor[1].plane = plane;
                            candidate_cursor[1].side = 1;
                            candidate_cursor++;
                            plane = plane->front;
                        }
                        else
                        {
                            candidate_cursor[1].plane = plane;
                            candidate_cursor[1].side = 3;
                            candidate_cursor++;
                            plane = plane->back;
                        }
                    }
                    else
                    {
                        plane = plane->back;
                    }
                }
                else
                {
                    plane = plane->front;
                }
            }
            if (plane != 0)
            {
                if (plane->scene_index == 0)
                    goto walk_tree;
                leaf_index = plane->scene_index;
                scene = world->scenes_by_index[leaf_index - 1];
                if (scene != 0 &&
                    EQ_CDECL(int, 0x10001EEE,
                             (SpriteCollisionState_guess *state, CollisionScene_guess *current_scene))(
                        &collision_state, scene) != 0)
                {
                    current_end_x = (end_x - start_x) * collision_state.fraction + start_x;
                    current_end_y = (end_y - start_y) * collision_state.fraction + start_y;
                    current_end_z = (end_z - start_z) * collision_state.fraction + start_z;
                }
            }
        }
        for (; world->traversal_stack <= candidate_cursor; candidate_cursor--)
        {
            plane = candidate_cursor->plane;
            candidate_side = candidate_cursor->side;
            if (candidate_side == 1)
            {
                start_plane_distance = start_y * plane->normal_y + start_z * plane->normal_z +
                                       start_x * plane->normal_x + plane->distance;
                end_plane_distance = current_end_y * plane->normal_y + current_end_z * plane->normal_z +
                                    current_end_x * plane->normal_x + plane->distance;
                if (start_plane_distance < radius || end_plane_distance < radius)
                {
                    plane = plane->back;
                    candidate_cursor->side = 4;
                    break;
                }
            }
            else if (candidate_side == 3)
            {
                start_plane_distance = start_y * plane->normal_y + start_z * plane->normal_z +
                                       start_x * plane->normal_x + plane->distance;
                end_plane_distance = current_end_y * plane->normal_y + current_end_z * plane->normal_z +
                                    current_end_x * plane->normal_x + plane->distance;
                if (-radius < start_plane_distance || -radius < end_plane_distance)
                {
                    plane = plane->front;
                    candidate_cursor->side = 2;
                    break;
                }
            }
        }
    } while (world->traversal_stack <= candidate_cursor);
    if (collision_state.fraction < EQ_GLOBAL(float, 0x10076284))
        collision_state.fraction = 0.0f;
    if (collision_state.hit_actor != 0)
    {
        projection = direction_x * offset_x + direction_y * offset_y + direction_z * offset_z;
        normal_x = direction_x - projection * offset_x;
        normal_y = direction_y - projection * offset_y;
        normal_z = direction_z - projection * offset_z;
        normal_squared = normal_x * normal_x + normal_y * normal_y + normal_z * normal_z;
        if (normal_squared > EQ_GLOBAL(float, 0x10076284))
        {
            inverse_normal_length = EQ_GLOBAL(MovingSphereInvSqrt_guess, 0x100FC77C)(normal_squared);
            normal_x = inverse_normal_length * normal_x;
            normal_y = inverse_normal_length * normal_y;
            normal_z = inverse_normal_length * normal_z;
            projection = normal_x * direction_x + normal_y * direction_y + normal_z * direction_z;
            if (projection < EQ_GLOBAL(float, 0x10076284))
                projection = -projection;
            penetration = (1.0f - collision_state.fraction) * projection * segment_length;
            normal_x = offset_x * EQ_GLOBAL(float, 0x10076288) + normal_x;
            normal_y = offset_y * EQ_GLOBAL(float, 0x10076288) + normal_y;
            normal_z = offset_z * EQ_GLOBAL(float, 0x10076288) + normal_z;
            normal_squared = normal_x * normal_x + normal_y * normal_y + normal_z * normal_z;
            inverse_normal_length = EQ_GLOBAL(MovingSphereInvSqrt_guess, 0x100FC77C)(normal_squared);
            normal_x = inverse_normal_length * normal_x;
            normal_y = inverse_normal_length * normal_y;
            normal_z = inverse_normal_length * normal_z;
        }
    }
    else
    {
        normal_x = 0.0f;
        normal_y = 0.0f;
        normal_z = 0.0f;
        penetration = 0.0f;
    }
    collision_result->hit_actor = collision_state.hit_actor;
    collision_result->hit_entry = collision_state.last_hit;
    collision_result->fraction = collision_state.fraction;
    collision_result->distance = segment_length * collision_state.fraction;
    collision_result->normal_x = normal_x;
    collision_result->normal_y = normal_y;
    collision_result->normal_z = normal_z;
    collision_result->penetration = penetration;
    collision_result->offset_x = offset_x;
    collision_result->offset_y = offset_y;
    collision_result->offset_z = offset_z;
    return 0;
}

// FUNCTION: EQGFX 0x10001EEE
// Tests the swept sphere against one BSP region: its listed actors by collision volume, then the region's mesh
int __cdecl s3dCollideMovingSphereWithActors_guess(SpriteCollisionState_guess *collision_state, CollisionScene_guess *scene)
{
    int result;
    int collision_result;
    int filter_result;
    int found_collision;
    unsigned int *actor_indices;
    unsigned int *actor_limit;
    unsigned int sprite_index;
    unsigned int polygon_index;
    unsigned int part_index;
    unsigned char *polygon_cursor;
    unsigned char *polygon_limit;
    ActorRegistry_guess *actor_registry;
    T3D_tagACTORINSTANCE *actor;
    ActorSprite_guess *actor_sprite;
    SpriteCollisionVolume_guess *collision_volume;
    SpriteCollisionData_guess *collision_data;
    CollisionModel_guess *collision_model;
    AnimatedCollisionData_guess *animated_data;
    StaticCollisionData_guess *static_data;
    HierarchicalSpriteObject_guess *hierarchical_sprite;
    void *polygon_owner;
    void *attached_sprite;
    void *final_object;
    float center_offset[3];
    float collision_center[3];
    float sphere_radius;
    float *sphere_center;
    float saved_segment[6];

    result = 0;
    actor_registry = (ActorRegistry_guess *)collision_state->actor_registry;
    actor_indices = scene->active_actors->indices;
    actor_limit = actor_indices + scene->active_actors->count;
    if (actor_indices < actor_limit)
    {
        do
        {
            sprite_index = *actor_indices;
            actor = actor_registry->actors_by_index[sprite_index];
            if (actor->collisionStatus_guess != EQGFX_COLLISION_COMPLETE_GUESS &&
                actor->collisionStatus_guess != EQGFX_COLLISION_BUSY_GUESS)
            {
                actor->collisionStatus_guess = EQGFX_COLLISION_BUSY_GUESS;
                if (collision_state->actor_filter != 0)
                {
                    filter_result = collision_state->actor_filter(actor, collision_state->filter_context);
                    if (filter_result == 0)
                    {
                        actor->collisionStatus_guess = EQGFX_COLLISION_COMPLETE_GUESS;
                        goto actor_done;
                    }
                    if (filter_result == 2)
                    {
                        sphere_center = (float *)((char *)actor + 0x10);
                        sphere_radius = actor->boundingRadius_guess * actor->scaleFactor_guess;
                        collision_result = EQGFX_TEST_SEGMENT_SPHERE_GUESS(collision_state, sphere_center, sphere_radius);
                        if (collision_result != 0)
                        {
                            if (collision_result != 1)
                            {
                                actor->collisionStatus_guess = EQGFX_COLLISION_COMPLETE_GUESS;
                                goto actor_done;
                            }
                            collision_state->last_hit = 0;
                            collision_state->hit_actor = actor;
                            result = 1;
                        }
                        goto actor_done;
                    }
                }

                collision_volume = (SpriteCollisionVolume_guess *)actor->ligList_guess;
                switch (collision_volume->type)
                {
                case 2:
                    collision_data = collision_volume->data;
                    collision_result = EQGFX_TEST_SPHERE_GUESS(
                        collision_state, (float *)((char *)actor + 0x10),
                        collision_data->radius * actor->scaleFactor_guess);
                    if (collision_result != 1)
                    {
                        if (collision_result == 2)
                            actor->collisionStatus_guess = EQGFX_COLLISION_COMPLETE_GUESS;
                        goto actor_done;
                    }
                    EQGFX_UPDATE_ACTOR_TRANSFORM_GUESS(
                        actor, actor->scaleFactor_guess, (float *)((char *)actor + 0x10));
                    EQGFX_BUILD_COLLISION_ORIGIN_GUESS(
                        actor, center_offset, collision_state, EQGFX_COLLISION_OUTPUT_GUESS);
                    if ((actor->flags_guess[3] & 0x10) != 0)
                    {
                        attached_sprite = (void *)*actor->attachedSprites_guess;
                        if (attached_sprite != 0)
                        {
                            if (*(int *)attached_sprite == 0x48)
                                collision_result = EQGFX_TEST_HIERARCHICAL_COLLISION_GUESS(
                                    EQGFX_COLLISION_OUTPUT_GUESS, attached_sprite);
                            else if (*(int *)attached_sprite == 0x14 &&
                                     (*(unsigned char *)((char *)attached_sprite + 0x0e) & 0x40) != 0)
                                collision_result = EQGFX_TEST_TRANSFORMED_SPRITE_GUESS(
                                    EQGFX_COLLISION_OUTPUT_GUESS, attached_sprite);
                            else
                                collision_result = 0;
                            if (collision_result != 0)
                            {
                                actor->collisionStatus_guess = EQGFX_COLLISION_COMPLETE_GUESS;
                                goto actor_done;
                            }
                        }
                    }
                    found_collision = 0;
                    polygon_cursor = (unsigned char *)collision_data->polygons;
                    polygon_limit = polygon_cursor + collision_data->polygon_count * 0x20;
                    if (polygon_cursor < polygon_limit)
                    {
                        do
                        {
                            collision_result = EQGFX_TEST_POLYGON_GUESS(
                                EQGFX_COLLISION_OUTPUT_GUESS, polygon_cursor);
                            if (collision_result != 0)
                                found_collision = 1;
                            polygon_cursor += 0x20;
                        } while (polygon_cursor < polygon_limit);
                        if (found_collision != 0)
                        {
                            collision_state->last_hit = 0;
                            collision_state->hit_actor = actor;
                            EQGFX_FINISH_COLLISION_GUESS(
                                actor, collision_state, EQGFX_COLLISION_OUTPUT_GUESS);
                            result = 1;
                        }
                    }
                    goto actor_done;

                case 0x2a:
                    sphere_center = (float *)((char *)actor + 0x10);
                    sphere_radius = collision_volume->parameters[1] * actor->scaleFactor_guess;
                    goto test_actor_sphere;

                case 0x2b:
                    sphere_radius = collision_volume->parameters[3] * actor->scaleFactor_guess;
                    collision_center[0] = collision_volume->parameters[0] * actor->scaleFactor_guess;
                    collision_center[1] = collision_volume->parameters[1] * actor->scaleFactor_guess;
                    collision_center[2] = collision_volume->parameters[2] * actor->scaleFactor_guess;
                    sphere_center = collision_center;
                    goto test_actor_sphere;

                case 0x2e:
                    actor_sprite = (ActorSprite_guess *)EQGFX_GET_ACTOR_SPRITE_GUESS(
                        actor, actor->currentAction_guess, 0);
                    EQGFX_GET_SPRITE_CENTER_GUESS(actor_sprite, center_offset);
                    EQGFX_GET_SPRITE_VOLUME_GUESS(
                        actor_sprite, (SpriteCollisionVolume_guess **)&actor_sprite);
                    collision_volume = (SpriteCollisionVolume_guess *)actor_sprite;
                    if (collision_volume == 0)
                        goto actor_done;
                    switch (collision_volume->type)
                    {
                    case 2:
                        EQGFX_GET_ACTOR_CENTER_GUESS(actor, center_offset, collision_center);
                        collision_result = EQGFX_TEST_SPHERE_GUESS(
                            collision_state, collision_center,
                            collision_volume->data->radius * actor->scaleFactor_guess);
                        if (collision_result != 1)
                        {
                            if (collision_result == 2)
                                actor->collisionStatus_guess = EQGFX_COLLISION_COMPLETE_GUESS;
                            goto actor_done;
                        }
                        EQGFX_UPDATE_ACTOR_TRANSFORM_GUESS(
                            actor, actor->scaleFactor_guess, (float *)((char *)actor + 0x10));
                        EQGFX_BUILD_COLLISION_ORIGIN_GUESS(
                            actor, center_offset, collision_state, EQGFX_COLLISION_OUTPUT_GUESS);
                        if ((actor->flags_guess[3] & 0x10) != 0)
                        {
                            attached_sprite = (void *)*actor->attachedSprites_guess;
                            if (attached_sprite != 0)
                            {
                                if (*(int *)attached_sprite == 0x48)
                                    collision_result = EQGFX_TEST_HIERARCHICAL_COLLISION_GUESS(
                                        EQGFX_COLLISION_OUTPUT_GUESS, attached_sprite);
                                else if (*(int *)attached_sprite == 0x14 &&
                                         (*(unsigned char *)((char *)attached_sprite + 0x0e) & 0x40) != 0)
                                    collision_result = EQGFX_TEST_TRANSFORMED_SPRITE_GUESS(
                                        EQGFX_COLLISION_OUTPUT_GUESS, attached_sprite);
                                else
                                    collision_result = 0;
                                if (collision_result != 0)
                                {
                                    actor->collisionStatus_guess = EQGFX_COLLISION_COMPLETE_GUESS;
                                    goto actor_done;
                                }
                            }
                        }
                        found_collision = 0;
                        polygon_cursor = (unsigned char *)collision_volume->data->polygons;
                        polygon_limit = polygon_cursor + collision_volume->data->polygon_count * 0x20;
                        if (polygon_cursor < polygon_limit)
                        {
                            do
                            {
                                collision_result = EQGFX_TEST_POLYGON_GUESS(
                                    EQGFX_COLLISION_OUTPUT_GUESS, polygon_cursor);
                                if (collision_result != 0)
                                    found_collision = 1;
                                polygon_cursor += 0x20;
                            } while (polygon_cursor < polygon_limit);
                            if (found_collision != 0)
                            {
                                collision_state->last_hit = 0;
                                collision_state->hit_actor = actor;
                                EQGFX_FINISH_COLLISION_GUESS(
                                    actor, collision_state, EQGFX_COLLISION_OUTPUT_GUESS);
                                result = 1;
                            }
                        }
                        goto actor_done;

                    case 0x2a:
                        EQGFX_GET_ACTOR_CENTER_GUESS(actor, center_offset, collision_center);
                        sphere_radius = collision_volume->parameters[1] * actor->scaleFactor_guess;
                        sphere_center = collision_center;
                        goto test_actor_sphere;

                    case 0x2b:
                        EQGFX_GET_ACTOR_CENTER_GUESS(actor, center_offset, collision_center);
                        sphere_radius = collision_volume->parameters[3] * actor->scaleFactor_guess;
                        collision_center[0] += collision_volume->parameters[0] * actor->scaleFactor_guess;
                        collision_center[1] += collision_volume->parameters[1] * actor->scaleFactor_guess;
                        collision_center[2] += collision_volume->parameters[2] * actor->scaleFactor_guess;
                        sphere_center = collision_center;
                        goto test_actor_sphere;

                    case 0x52:
                        actor_sprite = (ActorSprite_guess *)EQGFX_GET_ACTOR_SPRITE_GUESS(
                            actor, actor->currentAction_guess, 0);
                        if (actor_sprite->type != 0x48)
                            goto actor_done;
                        hierarchical_sprite = (HierarchicalSpriteObject_guess *)actor_sprite;
                        collision_model = hierarchical_sprite->collision_model;
                        EQGFX_GET_ACTOR_CENTER_GUESS(actor, center_offset, collision_center);
                        if (collision_model->animated_data != 0)
                            sphere_radius = ((AnimatedCollisionData_guess *)collision_model->animated_data)->radius;
                        else
                            sphere_radius = ((StaticCollisionData_guess *)collision_model->static_data)->radius;
                        collision_result = EQGFX_TEST_SPHERE_GUESS(
                            collision_state, collision_center, sphere_radius * actor->scaleFactor_guess);
                        if (collision_result != 1)
                        {
                            if (collision_result == 2)
                                actor->collisionStatus_guess = EQGFX_COLLISION_COMPLETE_GUESS;
                            goto actor_done;
                        }
                        EQGFX_UPDATE_ACTOR_TRANSFORM_GUESS(
                            actor, actor->scaleFactor_guess, (float *)((char *)actor + 0x10));
                        EQGFX_BUILD_COLLISION_ORIGIN_GUESS(
                            actor, center_offset, collision_state, EQGFX_COLLISION_OUTPUT_GUESS);
                        collision_result = EQGFX_TEST_HIERARCHICAL_COLLISION_GUESS(
                            EQGFX_COLLISION_OUTPUT_GUESS, actor_sprite);
                        if (collision_result != 0)
                        {
                            actor->collisionStatus_guess = EQGFX_COLLISION_COMPLETE_GUESS;
                            goto actor_done;
                        }
                        found_collision = 0;
                        if (collision_model->animated_data != 0)
                        {
                            animated_data = (AnimatedCollisionData_guess *)collision_model->animated_data;
                            if (animated_data->part_count != 0)
                            {
                                part_index = 0;
                                do
                                {
                                    collision_result = EQGFX_TEST_ANIMATED_PART_GUESS(
                                        EQGFX_COLLISION_OUTPUT_GUESS, collision_model, part_index);
                                    if (collision_result != 0)
                                        found_collision = 1;
                                    part_index++;
                                } while (part_index < animated_data->part_count);
                                goto sprite_model_polygons_done;
                            }
                        }
                        else
                        {
                            static_data = (StaticCollisionData_guess *)collision_model->static_data;
                            if (static_data->polygon_count != 0)
                            {
                                polygon_index = 0;
                                do
                                {
                                    collision_result = EQGFX_TEST_STATIC_POLYGON_GUESS(
                                        EQGFX_COLLISION_OUTPUT_GUESS, collision_model, polygon_index);
                                    if (collision_result != 0)
                                        found_collision = 1;
                                    polygon_index++;
                                } while (polygon_index < static_data->polygon_count);
                                goto sprite_model_polygons_done;
                            }
                        }
                        goto actor_done;
                    sprite_model_polygons_done:
                        if (found_collision != 0)
                        {
                            collision_state->last_hit = 0;
                            collision_state->hit_actor = actor;
                            EQGFX_FINISH_COLLISION_GUESS(
                                actor, collision_state, EQGFX_COLLISION_OUTPUT_GUESS);
                            result = 1;
                        }
                        goto actor_done;

                    case 0x54:
                        actor_sprite = (ActorSprite_guess *)EQGFX_GET_ACTOR_SPRITE_GUESS(
                            actor, actor->currentAction_guess, 0);
                        if (actor_sprite->type != 0x14)
                            goto actor_done;
                        collision_result = EQGFX_TEST_SPHERE_GUESS(
                            collision_state, (float *)((char *)actor + 0x10),
                            actor->boundingRadius_guess * actor->scaleFactor_guess);
                        if (collision_result == 1)
                        {
                            EQGFX_UPDATE_HIERARCHICAL_TRANSFORMS_GUESS(
                                actor_sprite, (int)(unsigned int)collision_state->actor_registry, actor);
                            collision_result = EQGFX_TEST_SPRITE_COLLISION_GUESS(
                                collision_state, actor, actor_sprite);
                            if (collision_result != 0)
                            {
                                result = 1;
                                collision_state->hit_actor = actor;
                            }
                        }
                        else if (collision_result == 2)
                            actor->collisionStatus_guess = EQGFX_COLLISION_COMPLETE_GUESS;
                        goto actor_done;
                    }
                    goto actor_done;
                }
            }
actor_done:
            actor_indices++;
        } while (actor_indices < actor_limit);
    }

    final_object = *(void **)((char *)scene->collision_source + 0x1c);
    if (final_object == 0)
        return result;
    if (*(int *)final_object == 0x48)
    {
        hierarchical_sprite = (HierarchicalSpriteObject_guess *)final_object;
        collision_model = hierarchical_sprite->collision_model;
        polygon_owner = collision_model;
        if (collision_model->animated_data != 0)
        {
            animated_data = (AnimatedCollisionData_guess *)collision_model->animated_data;
            if (animated_data->part_count == 0)
                return result;
            part_index = 0;
            do
            {
                collision_result = EQGFX_TEST_ANIMATED_PART_GUESS(
                    collision_state, collision_model, part_index);
                if (collision_result != 0)
                {
                    collision_state->last_hit = 0;
                    collision_state->hit_actor = scene;
                    result = 1;
                }
                part_index++;
            } while (part_index < animated_data->part_count);
            return result;
        }
        static_data = (StaticCollisionData_guess *)collision_model->static_data;
        if (static_data->polygon_count == 0)
            return result;
    }
    else
    {
        collision_volume = (SpriteCollisionVolume_guess *)final_object;
        static_data = (StaticCollisionData_guess *)collision_volume->data;
        polygon_owner = final_object;
        if (static_data->polygon_count == 0)
            return result;
    }

    saved_segment[0] = collision_state->segment_start[0];
    saved_segment[1] = collision_state->segment_start[1];
    saved_segment[2] = collision_state->segment_start[2];
    saved_segment[3] = collision_state->segment_end[0];
    saved_segment[4] = collision_state->segment_end[1];
    saved_segment[5] = collision_state->segment_end[2];
    collision_state->segment_start[0] -= static_data->origin[0];
    collision_state->segment_start[1] -= static_data->origin[1];
    collision_state->segment_start[2] -= static_data->origin[2];
    collision_state->segment_end[0] -= static_data->origin[0];
    collision_state->segment_end[1] -= static_data->origin[1];
    collision_state->segment_end[2] -= static_data->origin[2];
    polygon_index = 0;
    do
    {
        collision_result = EQGFX_TEST_STATIC_POLYGON_GUESS(
            collision_state, polygon_owner, polygon_index);
        if (collision_result != 0)
        {
            collision_state->last_hit = 0;
            collision_state->hit_actor = scene;
            result = 1;
        }
        polygon_index++;
    } while (polygon_index < static_data->polygon_count);
    collision_state->segment_start[0] = saved_segment[0];
    collision_state->segment_start[1] = saved_segment[1];
    collision_state->segment_start[2] = saved_segment[2];
    collision_state->segment_end[0] = saved_segment[3];
    collision_state->segment_end[1] = saved_segment[4];
    collision_state->segment_end[2] = saved_segment[5];
    return result;

test_actor_sphere:
    collision_result = EQGFX_TEST_SEGMENT_SPHERE_GUESS(
        collision_state, sphere_center, sphere_radius);
    if (collision_result != 0)
    {
        if (collision_result != 1)
        {
            actor->collisionStatus_guess = EQGFX_COLLISION_COMPLETE_GUESS;
            goto actor_done;
        }
        collision_state->last_hit = 0;
        collision_state->hit_actor = actor;
        result = 1;
    }
    goto actor_done;
}

// FUNCTION: EQGFX 0x100025d6
// Checks a moving sphere against the collision polygons of individual sprites in a sprite collection.
int __cdecl s3dCollideMovingSphereWithHSprite(SpriteCollisionState_guess *collision_state,
                         Origin_guess *transform,
                         SpriteCollisionList_guess *sprites)
{
    int result;
    int polygon_hit;
    unsigned int sprite_address;
    unsigned int sprite_end;
    unsigned int polygon_address;
    unsigned int polygon_end;
    SpriteCollisionEntry_guess *sprite_entry;
    void *sprite;
    SpriteCollisionVolume_guess *collision_volume;
    SpriteCollisionData_guess *collision_data;
    float center_offset[3];
    float center_bounds[3];
    unsigned char beam[104];

    result = 0;
    sprite_address = (unsigned int)sprites->entries;
    sprite_end = sprites->count * 0x148 + sprite_address;
    if (sprite_address < sprite_end)
    {
        do
        {
            sprite_entry = (SpriteCollisionEntry_guess *)sprite_address;
            sprite = sprite_entry->sprite;
            if (sprite != 0)
            {
                EQGFX_GET_SPRITE_CENTER_GUESS(sprite, center_offset);
                EQGFX_GET_SPRITE_VOLUME_GUESS
                    (sprite, &collision_volume);
                if (collision_volume != 0 && collision_volume->type == 2)
                {
                    EQ_CDECL(void, 0x100047C4,
                             (SpriteCollisionEntry_guess *, float *, float *))
                        (sprite_entry, center_offset, center_bounds);
                    collision_data = collision_volume->data;
                    if (EQ_CDECL(int, 0x1000306A, (void *, float *, float))
                        (collision_state, center_bounds,
                         collision_data->radius * sprite_entry->radius * transform->field_34) == 1)
                    {
                        EQ_CDECL(void, 0x1000291B,
                                 (SpriteCollisionEntry_guess *, Origin_guess *, float *,
                                  SpriteCollisionState_guess *, void *))
                            (sprite_entry, transform, center_offset, collision_state, beam);
                        polygon_hit = 0;
                        polygon_address = (unsigned int)collision_data->polygons;
                        polygon_end = collision_data->polygon_count * 0x20 + polygon_address;
                        if (polygon_address < polygon_end)
                        {
                            do
                            {
                                if (EQGFX_TEST_POLYGON_GUESS
                                    (beam, (void *)polygon_address) != 0)
                                {
                                    polygon_hit = 1;
                                }
                                polygon_address = polygon_address + 0x20;
                            } while (polygon_address < polygon_end);
                            if (polygon_hit != 0)
                            {
                                collision_state->last_hit = sprite_entry;
                                EQ_CDECL(void, 0x10002E9B,
                                         (SpriteCollisionEntry_guess *, Origin_guess *,
                                          SpriteCollisionState_guess *, void *))
                                    (sprite_entry, transform, collision_state, beam);
                                result = 1;
                            }
                        }
                    }
                }
            }
            sprite_address = sprite_address + 0x148;
        } while (sprite_address < sprite_end);
    }
    return result;
}

// FUNCTION: EQGFX 0x1000270A
// __cdecl, no value returned. Transforms both vertices and both directions; derives delta and the radius terms.
void __cdecl s3dTransformMovingSphereToObjectSpace_guess(PointTransform_guess *transform, float *origin_xyz, Beam_guess *src, Beam_guess *dst)
{
    float dx;
    float dy;
    float dz;
    float radius;

    dst->field_00 = src->field_00;
    dst->field_04 = src->field_04;

    dx = src->start[0] - transform->position[0];
    dy = src->start[1] - transform->position[1];
    dz = src->start[2] - transform->position[2];
    dst->start[0] = dx * transform->basis[0][0] + dz * transform->basis[2][0] + dy * transform->basis[1][0] -
                    origin_xyz[0];
    dst->start[1] = dx * transform->basis[0][1] + dz * transform->basis[2][1] + dy * transform->basis[1][1] -
                    origin_xyz[1];
    dst->start[2] = dx * transform->basis[0][2] + dz * transform->basis[2][2] + dy * transform->basis[1][2] -
                    origin_xyz[2];

    dx = src->end[0] - transform->position[0];
    dy = src->end[1] - transform->position[1];
    dz = src->end[2] - transform->position[2];
    dst->end[0] = dx * transform->basis[0][0] + dz * transform->basis[2][0] + dy * transform->basis[1][0] -
                  origin_xyz[0];
    dst->end[1] = dx * transform->basis[0][1] + dz * transform->basis[2][1] + dy * transform->basis[1][1] -
                  origin_xyz[1];
    dst->end[2] = dx * transform->basis[0][2] + dz * transform->basis[2][2] + dy * transform->basis[1][2] -
                  origin_xyz[2];

    dst->delta[0] = dst->end[0] - dst->start[0];
    dst->delta[1] = dst->end[1] - dst->start[1];
    dst->delta[2] = dst->end[2] - dst->start[2];

    radius = (1.0f / transform->field_34) * src->radius;
    dst->radius = radius;
    dst->radius_sq = radius * radius;
    // Reads the stored float back: the x87 register still holds the unrounded radius.
    dst->radius_inv = 1.0f / *(volatile float *)&dst->radius;

    dst->field_38 = src->field_38 * transform->field_34;
    dst->field_50 = src->field_50;

    dst->dir_a[0] = ((transform->direction_basis[0][0] * src->dir_a[0] + src->dir_a[2] * transform->direction_basis[0][2]) +
                    src->dir_a[1] * transform->direction_basis[0][1]);
    dst->dir_a[1] = ((src->dir_a[2] * transform->direction_basis[1][2] + src->dir_a[1] * transform->direction_basis[1][1]) +
                    transform->direction_basis[1][0] * src->dir_a[0]);
    dst->dir_a[2] = ((src->dir_a[2] * transform->direction_basis[2][2] + src->dir_a[1] * transform->direction_basis[2][1]) +
                    transform->direction_basis[2][0] * src->dir_a[0]);

    dst->dir_b[0] = ((src->dir_b[2] * transform->direction_basis[0][2] + src->dir_b[1] * transform->direction_basis[0][1]) +
                    transform->direction_basis[0][0] * src->dir_b[0]);
    dst->dir_b[1] = ((src->dir_b[0] * transform->direction_basis[1][0] + src->dir_b[2] * transform->direction_basis[1][2]) +
                    src->dir_b[1] * transform->direction_basis[1][1]);
    dst->dir_b[2] = ((src->dir_b[0] * transform->direction_basis[2][0] + src->dir_b[2] * transform->direction_basis[2][2]) +
                    src->dir_b[1] * transform->direction_basis[2][1]);

    dst->field_48 = src->field_48;
    dst->field_4c = src->field_4c;
}

// FUNCTION: EQGFX 0x1000291B
// __cdecl, no value returned. Transforms both vertices and both directions; scales the radius.
void __cdecl s3dTransformMovingSphereByMatrix_guess(Transform_guess *transform, Origin_guess *origin, float *origin_xyz,
                          Beam_guess *src, Beam_guess *dst)
{
    float x;
    float y;
    float z;
    float scaled_radius;
    float partial;

    dst->field_00 = src->field_00;
    dst->field_04 = src->field_04;

    x = src->start[0];
    y = src->start[1];
    z = src->start[2];
    dst->start[0] = x * transform->basis[0][0] + z * transform->basis[2][0] + y * transform->basis[1][0] +
                    transform->translation[0] - origin_xyz[0];
    dst->start[1] = x * transform->basis[0][1] + z * transform->basis[2][1] + y * transform->basis[1][1] +
                    transform->translation[1] - origin_xyz[1];
    dst->start[2] = x * transform->basis[0][2] + z * transform->basis[2][2] + y * transform->basis[1][2] +
                    transform->translation[2] - origin_xyz[2];

    x = src->end[0];
    y = src->end[1];
    z = src->end[2];
    dst->end[0] = x * transform->basis[0][0] + z * transform->basis[2][0] + y * transform->basis[1][0] +
                  transform->translation[0] - origin_xyz[0];
    dst->end[1] = x * transform->basis[0][1] + z * transform->basis[2][1] + y * transform->basis[1][1] +
                  transform->translation[1] - origin_xyz[1];
    dst->end[2] = x * transform->basis[0][2] + z * transform->basis[2][2] + y * transform->basis[1][2] +
                  transform->translation[2] - origin_xyz[2];

    dst->delta[0] = dst->end[0] - dst->start[0];
    dst->delta[1] = dst->end[1] - dst->start[1];
    dst->delta[2] = dst->end[2] - dst->start[2];

    scaled_radius = (1.0 / origin->field_34) * transform->field_54 * src->radius;
    dst->radius = scaled_radius;
    dst->radius_sq = scaled_radius * scaled_radius;
    // reads the stored (rounded) radius from memory, not the unrounded FPU copy of scaled_radius
    dst->radius_inv = 1.0 / *(volatile float *)&dst->radius;

    dst->field_38 = transform->field_38 * src->field_38 * origin->field_34;
    dst->field_50 = src->field_50;

    partial = src->dir_a[2] * transform->direction_basis[0][2] + src->dir_a[1] * transform->direction_basis[0][1];
    dst->dir_a[0] = partial + src->dir_a[0] * transform->direction_basis[0][0];
    partial = src->dir_a[0] * transform->direction_basis[1][0] + src->dir_a[2] * transform->direction_basis[1][2];
    dst->dir_a[1] = partial + src->dir_a[1] * transform->direction_basis[1][1];
    partial = src->dir_a[0] * transform->direction_basis[2][0] + src->dir_a[2] * transform->direction_basis[2][2];
    dst->dir_a[2] = partial + src->dir_a[1] * transform->direction_basis[2][1];

    partial = src->dir_b[0] * transform->direction_basis[0][0] + src->dir_b[2] * transform->direction_basis[0][2];
    dst->dir_b[0] = partial + src->dir_b[1] * transform->direction_basis[0][1];
    partial = src->dir_b[2] * transform->direction_basis[1][2] + src->dir_b[1] * transform->direction_basis[1][1];
    dst->dir_b[1] = partial + src->dir_b[0] * transform->direction_basis[1][0];
    partial = src->dir_b[2] * transform->direction_basis[2][2] + src->dir_b[1] * transform->direction_basis[2][1];
    dst->dir_b[2] = partial + src->dir_b[0] * transform->direction_basis[2][0];

    dst->field_48 = src->field_48;
    dst->field_4c = src->field_4c;
}

// FUNCTION: EQGFX 0x10002b6b
// Converts a ray segment into an object's collision space for hit testing.
// __cdecl, no value returned. Copies the source sprite's eight dword fields (the transform pointer lands at
// +0x4c) and writes both vertices as basis * (vertex - position) - origin, plus the vertex difference.
void __cdecl s3dTransformRaySegmentToObjectSpace_guess(PointTransform_guess *transform, float *origin, Sprite_guess *src, Sprite_guess *dst)
{
    float dx;
    float dy;
    float dz;

    dst->field_34 = src->field_34;
    dst->field_4c = (int)transform;
    dst->field_38 = src->field_38;
    dst->field_48 = src->field_48;
    dst->field_54 = src->field_54;
    dst->field_58 = src->field_58;
    dst->field_50 = src->field_50;
    dst->field_5c = src->field_5c;
    dst->field_40 = src->field_40;

    dx = src->vertex[0] - transform->position[0];
    dy = src->vertex[1] - transform->position[1];
    dz = src->vertex[2] - transform->position[2];
    dst->vertex[0] = dz * transform->basis[2][0] + dy * transform->basis[1][0] + dx * transform->basis[0][0] -
                     origin[0];
    dst->vertex[1] = dx * transform->basis[0][1] + dz * transform->basis[2][1] + dy * transform->basis[1][1] -
                     origin[1];
    dst->vertex[2] = dx * transform->basis[0][2] + dz * transform->basis[2][2] + dy * transform->basis[1][2] -
                     origin[2];

    dx = src->vertex[3] - transform->position[0];
    dy = src->vertex[4] - transform->position[1];
    dz = src->vertex[5] - transform->position[2];
    dst->vertex[3] = dz * transform->basis[2][0] + dy * transform->basis[1][0] + dx * transform->basis[0][0] -
                     origin[0];
    dst->vertex[4] = dx * transform->basis[0][1] + dz * transform->basis[2][1] + dy * transform->basis[1][1] -
                     origin[1];
    dst->vertex[5] = dx * transform->basis[0][2] + dz * transform->basis[2][2] + dy * transform->basis[1][2] -
                     origin[2];
    dst->vertex[6] = dst->vertex[3] - dst->vertex[0];
    dst->vertex[7] = dst->vertex[4] - dst->vertex[1];
    dst->vertex[8] = dst->vertex[5] - dst->vertex[2];
}

// File-local stand-ins for the object behind the first argument and for the object the source and the
// destination are. Nothing in include/ describes either, so their names and the fields between the ones the
// original touches are guesses.

typedef struct Basis_guess
{
    unsigned char unused_00[0xbc];
    float         basis[3][3];      // 0xbc  row r at 0xbc + 12*r; out = vector * basis
} Basis_guess;

typedef struct VectorState_guess
{
    unsigned char unused_00[0x8];
    float         position[3];      // 0x08  the point a passed-in point is measured from
    float         previous_position[3]; // 0x14  second point in the swept-plane test
    float         movement[3];      // 0x20  displacement applied to position
    float         normal[3];        // 0x2c  plane normal, x at 0x2c, y at 0x30, z at 0x34
    float         field_38;         // 0x38  scales the ray time into field_50
    float         radius;           // 0x3c  radius used by the polygon intersection
    float         field_40;         // 0x40  squared radius added to the discriminant
    float         field_44;         // 0x44  scales the stored vector on the way out
    unsigned char unused_48[0x50 - 0x48];
    float         field_50;         // 0x50  the stored distance; copied straight through
    unsigned char unused_54[0x5c - 0x54];
    float         field_5c;         // 0x5c  the vector rotated by the basis
    float         field_60;         // 0x60
    float         field_64;         // 0x64
} VectorState_guess;

// FUNCTION: EQGFX 0x10002e9b
// Copies a hit's fraction (+0x50) and rotates its contact normal (+0x5c) by the basis at +0xbc.
// __cdecl (four stack arguments, callee pops nothing; no value returned). Copies the source's float at +0x50,
// then rotates the source's three floats at +0x5c by the basis at +0xbc of `transform` into the destination.
void __cdecl transformVectorByBasis_guess(Basis_guess *transform, int unused, VectorState_guess *dst, VectorState_guess *src)
{
    float x;
    float y;
    float z;

    dst->field_50 = src->field_50;
    x = src->field_5c;
    y = src->field_60;
    z = src->field_64;
    dst->field_5c = z * transform->basis[2][0] + y * transform->basis[1][0] + x * transform->basis[0][0];
    dst->field_60 = z * transform->basis[2][1] + y * transform->basis[1][1] + x * transform->basis[0][1];
    dst->field_64 = z * transform->basis[2][2] + y * transform->basis[1][2] + x * transform->basis[0][2];
}

// File-local stand-in for the object behind the first argument. It carries the same row-vector basis as
// VectorState_guess's object, at +0x68 instead of +0xbc.

typedef struct Basis68_guess
{
    unsigned char unused_00[0x68];
    float         basis[3][3];      // 0x68  row r at 0x68 + 12*r; out = vector * basis
} Basis68_guess;

// FUNCTION: EQGFX 0x10002e32
// Returns an actor hit to world space: copies the fraction, rotates the contact normal by the actor's basis.
// __cdecl (three stack arguments, callee pops nothing; no value returned). Copies the source's float at
// +0x50, then rotates the source's three floats at +0x5c by the basis at +0x68 of `transform` into `dst`.
void __cdecl s3dTransformVectorState_guess(Basis68_guess *transform, VectorState_guess *dst, VectorState_guess *src)
{
    float x;
    float y;
    float z;

    dst->field_50 = src->field_50;
    x = src->field_5c;
    y = src->field_60;
    z = src->field_64;
    dst->field_5c = z * transform->basis[2][0] + y * transform->basis[1][0] + x * transform->basis[0][0];
    dst->field_60 = z * transform->basis[2][1] + y * transform->basis[1][1] + x * transform->basis[0][1];
    dst->field_64 = z * transform->basis[2][2] + y * transform->basis[1][2] + x * transform->basis[0][2];
}

// FUNCTION: EQGFX 0x10002cc3
// Prepares a collision ray for a sprite's coordinate frame using its transform and a reference origin.
// __cdecl (five stack arguments, callee pops nothing; no value returned). Copies nine dwords of the source
// sprite, row-vector-transforms its two vertices by the basis minus the origin, and stores the difference.
void __cdecl s3dTransformRaySegmentByMatrix_guess(Transform_guess *transform, Origin_guess *origin, int field_4c_value,
                          Sprite_guess *src, Sprite_guess *dst)
{
    float x;
    float y;
    float z;

    dst->field_34 = src->field_34;
    dst->field_38 = src->field_38;
    dst->field_48 = src->field_48;
    dst->field_54 = src->field_54;
    dst->field_58 = src->field_58;
    dst->field_50 = src->field_50;
    dst->field_5c = src->field_5c;
    dst->field_4c = field_4c_value;
    dst->field_40 = src->field_40;

    x = src->vertex[0];
    y = src->vertex[1];
    z = src->vertex[2];
    dst->vertex[0] = z * transform->basis[2][0] + y * transform->basis[1][0] + x * transform->basis[0][0] +
                     transform->translation[0] - origin->x;
    dst->vertex[1] = x * transform->basis[0][1] + z * transform->basis[2][1] + y * transform->basis[1][1] +
                     transform->translation[1] - origin->y;
    dst->vertex[2] = x * transform->basis[0][2] + z * transform->basis[2][2] + y * transform->basis[1][2] +
                     transform->translation[2] - origin->z;

    x = src->vertex[3];
    y = src->vertex[4];
    z = src->vertex[5];
    dst->vertex[3] = z * transform->basis[2][0] + y * transform->basis[1][0] + x * transform->basis[0][0] +
                     transform->translation[0] - origin->x;
    dst->vertex[4] = x * transform->basis[0][1] + z * transform->basis[2][1] + y * transform->basis[1][1] +
                     transform->translation[1] - origin->y;
    dst->vertex[5] = x * transform->basis[0][2] + z * transform->basis[2][2] + y * transform->basis[1][2] +
                     transform->translation[2] - origin->z;

    dst->vertex[6] = dst->vertex[3] - dst->vertex[0];
    dst->vertex[7] = dst->vertex[4] - dst->vertex[1];
    dst->vertex[8] = dst->vertex[5] - dst->vertex[2];
}

typedef float (__cdecl *ReciprocalSqrtFn_guess)(float);

#include "trilogy/eqaddr.h"

// 0x10076284 the zero float every FP compare below reads (eqgfx_dx7.dll's .rdata); through its address, as the
// original's FCOM/FCOMP operand.
#define EQGFX_ZERO_F_GUESS          EQ_GLOBAL(float, 0x10076284)
#define EQGFX_SPAWN_AXIS_EPS_GUESS EQ_GLOBAL(float, 0x1007628C)

// 0x100fc77c the inverse-square-root routine the renderer installs at start-up (its C fallback
// t3dFloatInvSqrt_C is at 0x1004efda): one float on the stack, the result in ST0, called through the pointer's
// address like the original's `CALL dword ptr [0x100fc77c]`.
typedef float (__cdecl *InvSqrtFunc_guess)(float squared_length);
#define EQGFX_INV_SQRT_GUESS        EQ_GLOBAL(InvSqrtFunc_guess, 0x100FC77C)

// FUNCTION: EQGFX 0x10003331
// Finds a moving sphere's contact with a single vertex and updates the nearest contact normal.
// __cdecl (two stack arguments, callee pops nothing, returns a value). Ray-sphere: the point is tested against
// the state's plane and, when the hit is nearer than the stored distance, the state's distance and vector
// fields are rewritten and 1 is returned; 0 otherwise.
int __cdecl s3dCollideMovingSphereWithVertex(VectorState_guess *state, float *point)
{
    float plane_dot;
    float dx;
    float dy;
    float dz;
    float discriminant;
    float distance;
    float ray_time;

    dx = state->position[0] - point[0];
    dy = state->position[1] - point[1];
    dz = state->position[2] - point[2];
    plane_dot = dx * state->normal[0] + dz * state->normal[2] + dy * state->normal[1];
    if (plane_dot < EQGFX_ZERO_F_GUESS)
    {
        discriminant = plane_dot * plane_dot - ((dz * dz + dy * dy + dx * dx) - state->field_40);
        if (discriminant > EQGFX_ZERO_F_GUESS)
        {
            float scale;
            ray_time = -(EQ_CDECL(float, 0x1004EF68, (float value))(discriminant) + plane_dot);
            distance = ray_time * state->field_38;
            if (distance < state->field_50)
            {
                if (distance >= EQGFX_ZERO_F_GUESS)
                {
                    state->field_50 = distance;
                    state->field_5c = ray_time * state->normal[0] + state->position[0] - point[0];
                    state->field_60 = ray_time * state->normal[1] + state->position[1] - point[1];
                    state->field_64 = ray_time * state->normal[2] + state->position[2] - point[2];
                    state->field_5c = state->field_5c * state->field_44;
                    state->field_60 = state->field_60 * state->field_44;
                    state->field_64 = state->field_64 * state->field_44;
                }
                else
                {
                    state->field_50 = 0;
                    state->field_5c = state->position[0] - point[0];
                    state->field_60 = state->position[1] - point[1];
                    state->field_64 = state->position[2] - point[2];
                    scale = EQGFX_INV_SQRT_GUESS(state->field_60 * state->field_60 +
                                                 state->field_5c * state->field_5c +
                                                 state->field_64 * state->field_64);
                    state->field_5c = scale * state->field_5c;
                    state->field_60 *= scale;
                    state->field_64 = scale * state->field_64;
                }
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x100043fd
// Checks whether a potential collision point falls within a triangle's projected boundary.
// __cdecl (six stack arguments, callee pops nothing). 1 when `point` lies on the inner side of all three edges
// of triangle v0,v1,v2 in the plane `flags` picks (bit 2: y/z and normal[0], bit 3: x/z and normal[1], else x/y).
int __cdecl s3dCheckPointInProjectedTriangle_guess(float *point, unsigned char flags, float *normal, float *v0, float *v1, float *v2)
{
    if ((flags & 4) != 0)
    {
        if (EQGFX_ZERO_F_GUESS < normal[0])
        {
            if (EQGFX_ZERO_F_GUESS > (point[1] - v2[1]) * (v0[2] - v2[2]) - (v0[1] - v2[1]) * (point[2] - v2[2]))
                return 0;
            if (EQGFX_ZERO_F_GUESS > (point[1] - v0[1]) * (v1[2] - v0[2]) - (point[2] - v0[2]) * (v1[1] - v0[1]))
                return 0;
            if (EQGFX_ZERO_F_GUESS > (point[1] - v1[1]) * (v2[2] - v1[2]) - (point[2] - v1[2]) * (v2[1] - v1[1]))
                return 0;
        }
        else
        {
            if (EQGFX_ZERO_F_GUESS > (v2[2] - v0[2]) * (point[1] - v2[1]) - (point[2] - v2[2]) * (v2[1] - v0[1]))
                return 0;
            if (EQGFX_ZERO_F_GUESS > (point[1] - v0[1]) * (v0[2] - v1[2]) - (v0[1] - v1[1]) * (point[2] - v0[2]))
                return 0;
            if (EQGFX_ZERO_F_GUESS > (v1[2] - v2[2]) * (point[1] - v1[1]) - (point[2] - v1[2]) * (v1[1] - v2[1]))
                return 0;
        }
        return 1;
    }
    if ((flags & 8) != 0)
    {
        if (EQGFX_ZERO_F_GUESS < normal[1])
        {
            if (EQGFX_ZERO_F_GUESS > (v0[0] - v2[0]) * (point[2] - v2[2]) - (v0[2] - v2[2]) * (point[0] - v2[0]))
                return 0;
            if (EQGFX_ZERO_F_GUESS > (point[2] - v0[2]) * (v1[0] - v0[0]) - (point[0] - v0[0]) * (v1[2] - v0[2]))
                return 0;
            if (EQGFX_ZERO_F_GUESS > (v2[0] - v1[0]) * (point[2] - v1[2]) - (point[0] - v1[0]) * (v2[2] - v1[2]))
                return 0;
        }
        else
        {
            if (EQGFX_ZERO_F_GUESS > (v2[0] - v0[0]) * (point[2] - v2[2]) - (v2[2] - v0[2]) * (point[0] - v2[0]))
                return 0;
            if (EQGFX_ZERO_F_GUESS > (v0[0] - v1[0]) * (point[2] - v0[2]) - (point[0] - v0[0]) * (v0[2] - v1[2]))
                return 0;
            if (EQGFX_ZERO_F_GUESS > (point[2] - v1[2]) * (v1[0] - v2[0]) - (v1[2] - v2[2]) * (point[0] - v1[0]))
                return 0;
        }
        return 1;
    }
    if (EQGFX_ZERO_F_GUESS < normal[2])
    {
        if (EQGFX_ZERO_F_GUESS > (v0[1] - v2[1]) * (point[0] - v2[0]) - (v0[0] - v2[0]) * (point[1] - v2[1]))
            return 0;
        if (EQGFX_ZERO_F_GUESS > (point[0] - v0[0]) * (v1[1] - v0[1]) - (point[1] - v0[1]) * (v1[0] - v0[0]))
            return 0;
        if (EQGFX_ZERO_F_GUESS > (point[0] - v1[0]) * (v2[1] - v1[1]) - (v2[0] - v1[0]) * (point[1] - v1[1]))
            return 0;
    }
    else
    {
        if (EQGFX_ZERO_F_GUESS > (v2[1] - v0[1]) * (point[0] - v2[0]) - (v2[0] - v0[0]) * (point[1] - v2[1]))
            return 0;
        if (EQGFX_ZERO_F_GUESS > (v0[1] - v1[1]) * (point[0] - v0[0]) - (v0[0] - v1[0]) * (point[1] - v0[1]))
            return 0;
        if (EQGFX_ZERO_F_GUESS > (v1[1] - v2[1]) * (point[0] - v1[0]) - (v1[0] - v2[0]) * (point[1] - v1[1]))
            return 0;
    }
    return 1;
}

// FUNCTION: EQGFX 0x100046ed
// Converts an actor-local offset into its world-space point.
void __cdecl t3dTransformActorOffsetToWorld_guess(Transform_guess *transform, float *input, float *result)
{
    if ((*input == EQGFX_ZERO_F_GUESS) &&
        (input[1] == EQGFX_ZERO_F_GUESS) &&
        (input[2] == EQGFX_ZERO_F_GUESS))
    {
        *result = transform->position[0];
        result[1] = transform->position[1];
        result[2] = transform->position[2];
    }
    else
    {
        EQ_CDECL(void, 0x10035058, (Transform_guess *, float, float *))
            (transform, transform->field_34, transform->position);
        *result = transform->basis[2][0] * input[2] +
                  transform->basis[1][0] * input[1] +
                  transform->basis[0][0] * input[0] +
                  transform->position[0];
        result[1] = transform->basis[2][1] * input[2] +
                    transform->basis[1][1] * input[1] +
                    transform->basis[0][1] * input[0] +
                    transform->position[1];
        result[2] = transform->basis[2][2] * input[2] +
                    transform->basis[1][2] * input[1] +
                    transform->basis[0][2] * input[0] +
                    transform->position[2];
    }
    return;
}

// FUNCTION: EQGFX 0x100047c4
// Maps a point through the collision object's affine transform.
// __cdecl (three stack arguments, callee pops nothing, no meaningful EAX). Transforms `point` by `transform`:
// result = point.x * matrix[0] + point.y * matrix[1] + point.z * matrix[2] + offset.
void __cdecl transformPointByAffineTransform_guess(Transform_guess *transform, float *point, float *result)
{
    float x = point[0];
    float y = point[1];
    float z = point[2];

    result[0] = z * transform->matrix[2][0] + y * transform->matrix[1][0] + x * transform->matrix[0][0] + transform->offset[0];
    result[1] = z * transform->matrix[2][1] + y * transform->matrix[1][1] + x * transform->matrix[0][1] + transform->offset[1];
    result[2] = z * transform->matrix[2][2] + y * transform->matrix[1][2] + x * transform->matrix[0][2] + transform->offset[2];
}

// File-local stand-ins for the objects this function walks (the tested segment, the collision model with its
// entry, plane, vertex and surface arrays and the animated-texture table). Nothing in include/ describes
// them, so the names and the fields between the ones the original touches are guesses.

typedef struct TextureTable_guess
{
    unsigned char   unused_00[0x184];
    unsigned int    timing_value;           // 0x184  timing input for dynamic-mesh animation
    unsigned char   unused_188[0x3e0 - 0x188];
    unsigned int   *textures;              // 0x3e0  indexed by surface->texture's low 31 bits
    unsigned char   unused_3e4[0x610 - 0x3e4];
    float          *vertices;              // 0x610  three floats per vertex
} TextureTable_guess;

typedef struct CollisionObjectContext_guess
{
    unsigned char   unused_00[0x17c];
    void          **objects_by_index;       // 0x17c
} CollisionObjectContext_guess;

typedef struct CollisionIndexList_guess
{
    unsigned char   unused_00[0x04];
    unsigned int    count;                  // 0x04
    unsigned char   unused_08[0x0c - 0x08];
    unsigned int   *indices;                // 0x0c
} CollisionIndexList_guess;

typedef struct CollisionResultRecord_guess
{
    unsigned char   unused_00[0x1c];
    void           *collision;              // 0x1c
} CollisionResultRecord_guess;

typedef struct CollisionQueryContext_guess
{
    unsigned char   unused_00[0x0c];
    unsigned int    flags;                  // 0x0c  mask tested by the ray query
    unsigned char   unused_10[0x24 - 0x10];
    CollisionResultRecord_guess *result_record; // 0x24
    unsigned char   unused_28[0x2c - 0x28];
    CollisionIndexList_guess *candidate_list;   // 0x2c
} CollisionQueryContext_guess;

typedef struct ClipPlane_guess ClipPlane_guess;
struct ClipPlane_guess
{
    float           coefficients[4];        // 0x00
    ClipPlane_guess *positive_child;         // 0x10
    ClipPlane_guess *negative_child;         // 0x14
    int             collision_index;         // 0x18  zero for a branch, otherwise a 1-based context index
};

typedef struct ClipSegmentStackEntry_guess
{
    ClipPlane_guess *plane;                  // 0x00
    int             side;                    // 0x04  pending branch state
    float           point[3];                // 0x08  saved segment endpoint
    float           fraction;                // 0x14  saved endpoint fraction
} ClipSegmentStackEntry_guess;

typedef struct ClipWorld_guess
{
    unsigned char   unused_00[0x0c];
    ClipPlane_guess *planes;                 // 0x0c
    unsigned char   unused_10[0x24 - 0x10];
    CollisionQueryContext_guess **contexts;  // 0x24  indexed from one
    unsigned char   unused_28[0x17c - 0x28];
    void          **objects_by_index;        // 0x17c
    unsigned char   unused_180[0x35c - 0x180];
    ClipSegmentStackEntry_guess *stack_end;  // 0x35c
} ClipWorld_guess;

typedef struct CollisionLink_guess
{
    void           *collision;              // 0x00
} CollisionLink_guess;

typedef struct CollisionInstance_guess
{
    unsigned char   unused_00[0x0f];
    unsigned char   flags;                  // 0x0f
    float           transform[9];           // 0x10
    float           scale;                  // 0x34
    float           radius;                 // 0x38
    unsigned char   unused_3c[0x44 - 0x3c];
    CollisionLink_guess *collision_link;    // 0x44
    unsigned char   unused_48[0xf8 - 0x48];
    int             query_state;            // 0xf8
} CollisionInstance_guess;

typedef struct CollisionWorldTransform_guess
{
    unsigned char   unused_00[0x2c];
    float           translation[3];         // 0x2c
    unsigned char   unused_38[0x48 - 0x38];
    int             mesh_index;             // 0x48
} CollisionWorldTransform_guess;

typedef struct RayCollisionScratch_guess
{
    float           transformed_bounds[3];
    float           saved_ray[6];
    float           center_offset[3];
} RayCollisionScratch_guess;

typedef struct Ray_guess
{
    float           start[3];              // 0x00  the tested segment's start
    float           end[3];                // 0x0c  its end
    float           direction[3];          // 0x18  start + t * direction is the intersection point
    float           scaled_direction[3];    // 0x24  direction multiplied by the segment length
    float           segment_length;         // 0x30
    CollisionObjectContext_guess *object_context; // 0x34
    TextureTable_guess *texture_table;     // 0x38
    int           (*sprite_callback)(void *sprite, void *arg); // 0x3c
    int           (*callback)(void *context, unsigned int texture, void *arg);   // 0x40  vetoes an entry
    void           *callback_arg;          // 0x44
    int             test_all;              // 0x48  1 = also test entries with the 0x11 flag bits set
    void           *callback_context;      // 0x4c
    float           best_distance;         // 0x50  closest hit so far
    void           *hit_sprite;             // 0x54
    void           *hit_object;            // 0x58  object or DAG entry that produced the hit
    int             hit_entry_number;      // 0x5c  1-based number of the entry that hit
} Ray_guess;

typedef struct RayCollisionResult_guess
{
    void           *hit_sprite;             // 0x00
    void           *hit_object;             // 0x04
    unsigned int    surface_mask;           // 0x08
    int             entry_number;           // 0x0c
    float           distance;               // 0x10
} RayCollisionResult_guess;

typedef struct Surface_guess
{
    unsigned char   unused_00[0x14];
    unsigned int    texture;               // 0x14  top bit set = index into ray->texture_table
    unsigned char   unused_18[0x20 - 0x18];
    int             double_sided;          // 0x20  1 = the reversed plane crossing hits too
} Surface_guess;

typedef struct SurfaceList_guess
{
    unsigned char   unused_00[0x18];
    Surface_guess **surfaces;              // 0x18
} SurfaceList_guess;

typedef struct TriEntry_guess
{
    int             unused_00;             // 0x00
    int             surface_index;         // 0x04
    unsigned short  flags;                 // 0x08  bits 0/4 skip when not test_all; 4/8 pick the plane
    unsigned short  vertex0;               // 0x0a  indices into the mesh's vertex array
    unsigned short  vertex1;               // 0x0c
    unsigned short  vertex2;               // 0x0e
} TriEntry_guess;

typedef struct CollisionMesh_guess
{
    unsigned char   unused_00[0x04];
    float          *vertices;              // 0x04  three floats per vertex
    unsigned char   unused_08[0x20 - 0x08];
    int             triangle_count;        // 0x20
    TriEntry_guess *triangles;             // 0x24
    unsigned char   unused_28[0x2c - 0x28];
    float          *planes;                // 0x2c  one 4-float plane per entry
    float           bounding_radius;       // 0x30
} CollisionMesh_guess;

typedef struct MeshHolder_guess
{
    unsigned char   unused_00[0x14];
    CollisionMesh_guess *mesh;             // 0x14
    void           *alternate_mesh;        // 0x18
} MeshHolder_guess;

typedef struct DynamicFrameSet_guess
{
    unsigned char   unused_00[0x08];
    float         **vertices_by_frame;     // 0x08
} DynamicFrameSet_guess;

typedef struct DynamicMeshData_guess
{
    unsigned char   unused_00[0x14];
    DynamicFrameSet_guess *frames;          // 0x14
    MeshVertexStream_guess *vertex_stream;  // 0x18  packed animated short vertices
} DynamicMeshData_guess;

typedef struct DynamicSprite_guess
{
    unsigned char   unused_00[0x18];
    int             frame_index;            // 0x18
    unsigned char   unused_1c[0x34 - 0x1c];
    DynamicMeshData_guess *mesh_data;       // 0x34
} DynamicSprite_guess;

typedef struct Collision_guess
{
    int             type;                  // 0x00  0x48 for a collision object
    unsigned char   unused_04[0x18 - 0x04];
    CollisionWorldTransform_guess *world_transform; // 0x18 for non-0x48 collision types
    unsigned char   unused_1c[0x20 - 0x1c];
    SurfaceList_guess *surface_list;       // 0x20
    DynamicSprite_guess *dynamic_sprite;    // 0x24  present for a dynamic mesh collision
    unsigned char   unused_28[0x34 - 0x28];
    MeshHolder_guess  *mesh_holder;        // 0x34
} Collision_guess;

typedef struct HierarchicalSprite_guess
{
    int             type;                  // 0x00  0x14 for a hierarchical object
    unsigned char   unused_04[0x20 - 0x04];
    unsigned int    entry_count;            // 0x20
    void           *entries;               // 0x24  records are 0x148 bytes
    unsigned char   unused_28[0x40 - 0x28];
    unsigned int    child_count;            // 0x40
    unsigned char   unused_44[0x48 - 0x44];
    Collision_guess **children;             // 0x48
} HierarchicalSprite_guess;

typedef struct CollisionTreeEntry_guess
{
    unsigned char   unused_00[0x08];
    void           *object;                 // 0x08  collision or hierarchical object
    unsigned char   unused_0c[0x148 - 0x0c];
} CollisionTreeEntry_guess;

typedef struct AlternateMesh_guess
{
    unsigned char   unused_00[0x2c];
    float           translation[3];         // 0x2c
    unsigned char   unused_38[0x58 - 0x38];
    float           bounding_radius;        // 0x58
    unsigned char   unused_5c[0x74 - 0x5c];
    void           *collision_tree;         // 0x74
} AlternateMesh_guess;

#define EQGFX_COLLISION_BUSY EQ_GLOBAL(int, 0x10086080)

typedef float (__cdecl *FloatSqrtFunction_guess)(float value);
#define EQGFX_FLOAT_SQRT EQ_GLOBAL(FloatSqrtFunction_guess, 0x100FC77C)
#define EQGFX_REALLOCATE_CLIP_STACK EQ_CDECL(int, 0x10006E31, (ClipWorld_guess *))
#define EQGFX_TEST_CLIP_COLLISION EQ_CDECL(int, 0x10004BF1, (Ray_guess *, CollisionQueryContext_guess *))

// FUNCTION: EQGFX 0x1000482E
// Traces a segment through the world BSP to its nearest hit, or until a region's flags match surfaceMask
// @early-stop: 24 instruction differences remain; first difference: #12: original `jne @360` | rewrite `jne @356`.
/* fake match: invert the plane test for block order and reorder side 3's independent stores to keep VC6 from sharing restore tails. */
unsigned int __cdecl s3dCollideRaySegmentWithWorld(
    ClipWorld_guess *world,
    TextureTable_guess *textureTable,
    float *segmentStart,
    float *segmentEnd,
    unsigned int surfaceMask,
    int testAll,
    int (*spriteCallback)(void *, void *),
    int (*collisionCallback)(void *, unsigned int, void *),
    void *callbackArg,
    RayCollisionResult_guess *collisionOutput)
{
    unsigned int resultMask;
    int stackStatus;
    Ray_guess ray;
    float currentStart[3];
    float currentEnd[3];
    float currentStartFraction;
    float currentEndFraction;
    float startPlaneDistance;
    float endPlaneDistance;
    float crossingFraction;
    float savedPoint[3];
    float savedFraction;
    ClipPlane_guess *plane;
    ClipSegmentStackEntry_guess *stackEntry;
    CollisionQueryContext_guess *collisionContext;
    unsigned int collisionFlags;
    int collisionStatus;

    resultMask = 0;
    stackStatus = EQGFX_REALLOCATE_CLIP_STACK(world);
    if (stackStatus == 0)
    {
        ++EQGFX_COLLISION_BUSY;
        EQGFX_COLLISION_COMPLETE_GUESS = EQGFX_COLLISION_BUSY;
        ++EQGFX_COLLISION_BUSY;

        memcpy(ray.start, segmentStart, sizeof(ray.start));
        currentStartFraction = 0.0f;
        memcpy(currentStart, segmentStart, sizeof(currentStart));
        memcpy(ray.end, segmentEnd, sizeof(ray.end));
        memcpy(currentEnd, segmentEnd, sizeof(currentEnd));
        currentEndFraction = 1.0f;

        ray.direction[0] = ray.end[0] - ray.start[0];
        ray.direction[1] = ray.end[1] - ray.start[1];
        ray.direction[2] = ray.end[2] - ray.start[2];
        ray.segment_length = EQGFX_FLOAT_SQRT(
            ray.direction[0] * ray.direction[0] +
            ray.direction[1] * ray.direction[1] +
            ray.direction[2] * ray.direction[2]);
        ray.scaled_direction[0] = ray.segment_length * ray.direction[0];
        ray.scaled_direction[1] = ray.segment_length * ray.direction[1];
        ray.scaled_direction[2] = ray.segment_length * ray.direction[2];
        ray.object_context = (CollisionObjectContext_guess *)world;
        ray.texture_table = textureTable;
        ray.test_all = testAll;
        ray.sprite_callback = spriteCallback;
        ray.callback = collisionCallback;
        ray.callback_context = 0;
        ray.best_distance = 1.0f;
        ray.hit_sprite = 0;
        ray.hit_object = 0;
        ray.callback_arg = callbackArg;
        ray.hit_entry_number = 0;

        plane = world->planes;
        stackEntry = world->stack_end - 1;

    process_plane:
        do
        {
            if (currentStartFraction > ray.best_distance)
                break;

            if (plane != 0)
            {
                if (plane->collision_index == 0)
                {
                    startPlaneDistance =
                        (currentStart[1] * plane->coefficients[1] +
                         currentStart[2] * plane->coefficients[2]) +
                        currentStart[0] * plane->coefficients[0] + plane->coefficients[3];
                    endPlaneDistance =
                        (currentEnd[1] * plane->coefficients[1] +
                         currentEnd[2] * plane->coefficients[2]) +
                        currentEnd[0] * plane->coefficients[0] + plane->coefficients[3];
                    if (!(startPlaneDistance < EQGFX_ZERO_F_GUESS))
                    {
                        if (endPlaneDistance < EQGFX_ZERO_F_GUESS)
                        {
                            crossingFraction = startPlaneDistance / (startPlaneDistance - endPlaneDistance);
                            ++stackEntry;
                            stackEntry->plane = plane;
                            stackEntry->side = 1;
                            memcpy(stackEntry->point, currentEnd, sizeof(currentEnd));
                            stackEntry->fraction = currentEndFraction;
                            currentEndFraction = (currentEndFraction - currentStartFraction) * crossingFraction + currentStartFraction;
                            currentEnd[0] = (currentEnd[0] - currentStart[0]) * crossingFraction + currentStart[0];
                            currentEnd[1] = (currentEnd[1] - currentStart[1]) * crossingFraction + currentStart[1];
                            currentEnd[2] = (currentEnd[2] - currentStart[2]) * crossingFraction + currentStart[2];
                        }
                        plane = plane->positive_child;
                    }
                    else
                    {
                        if (endPlaneDistance >= EQGFX_ZERO_F_GUESS)
                        {
                            crossingFraction = startPlaneDistance / (startPlaneDistance - endPlaneDistance);
                            ++stackEntry;
                            stackEntry->plane = plane;
                            stackEntry->side = 3;
                            memcpy(stackEntry->point, currentEnd, sizeof(currentEnd));
                            stackEntry->fraction = currentEndFraction;
                            currentEndFraction = (currentEndFraction - currentStartFraction) * crossingFraction + currentStartFraction;
                            currentEnd[0] = (currentEnd[0] - currentStart[0]) * crossingFraction + currentStart[0];
                            currentEnd[1] = (currentEnd[1] - currentStart[1]) * crossingFraction + currentStart[1];
                            currentEnd[2] = (currentEnd[2] - currentStart[2]) * crossingFraction + currentStart[2];
                        }
                        plane = plane->negative_child;
                    }
                }

                if (plane != 0)
                {
                    if (plane->collision_index == 0)
                    {
                        /* fake match: restart plane testing before restoring the clip stack. */
                        goto process_plane;
                    }

                    collisionContext = world->contexts[plane->collision_index - 1];
                    collisionFlags = collisionContext->flags & surfaceMask;
                    if (collisionFlags != 0)
                    {
                        if (currentStartFraction < ray.best_distance)
                        {
                            resultMask = collisionFlags;
                            ray.best_distance = currentStartFraction;
                        }
                        collisionOutput->hit_sprite = ray.hit_sprite;
                        collisionOutput->hit_object = ray.hit_object;
                        collisionOutput->entry_number = ray.hit_entry_number - 1;
                        collisionOutput->surface_mask = collisionContext->flags & surfaceMask;
                        collisionOutput->distance = ray.best_distance;
                        return resultMask;
                    }

                    collisionStatus = EQGFX_TEST_CLIP_COLLISION(&ray, collisionContext);
                    if (collisionStatus != 0)
                        resultMask = 1;
                }
            }

            while (stackEntry >= world->stack_end)
            {
                plane = stackEntry->plane;
                if (stackEntry->side == 1)
                {
                    plane = plane->negative_child;
                    stackEntry->side = 4;
                    memcpy(savedPoint, stackEntry->point, sizeof(savedPoint));
                    savedFraction = stackEntry->fraction;
                    memcpy(stackEntry->point, currentStart, sizeof(currentStart));
                    memcpy(currentStart, currentEnd, sizeof(currentStart));
                    stackEntry->fraction = currentStartFraction;
                    currentStartFraction = currentEndFraction;
                    memcpy(currentEnd, savedPoint, sizeof(currentEnd));
                    currentEndFraction = savedFraction;
                    break;
                }
                else if (stackEntry->side == 3)
                {
                    plane = plane->positive_child;
                    stackEntry->side = 2;
                    memcpy(savedPoint, stackEntry->point, sizeof(savedPoint));
                    savedFraction = stackEntry->fraction;
                    memcpy(stackEntry->point, currentStart, sizeof(currentStart));
                    memcpy(currentStart, currentEnd, sizeof(currentStart));
                    stackEntry->fraction = currentStartFraction;
                    currentStartFraction = currentEndFraction;
                    currentEndFraction = savedFraction;
                    memcpy(currentEnd, savedPoint, sizeof(currentEnd));
                    break;
                }
                else
                {
                    memcpy(currentStart, stackEntry->point, sizeof(currentStart));
                    currentStartFraction = stackEntry->fraction;
                    --stackEntry;
                }
            }
        } while (stackEntry >= world->stack_end);

        collisionOutput->hit_sprite = ray.hit_sprite;
        collisionOutput->hit_object = ray.hit_object;
        collisionOutput->entry_number = ray.hit_entry_number - 1;
        collisionOutput->surface_mask = 0;
        collisionOutput->distance = ray.best_distance;
    }
    return resultMask;
}

// FUNCTION: EQGFX 0x10004BF1
// Tests a ray segment against each unvisited candidate object, then the context's mesh; records nearest hit
int __cdecl s3dCollideRaySegmentWithRegion_guess(Ray_guess *ray, CollisionQueryContext_guess *collisionContext)
{
    RayCollisionScratch_guess scratch;
    CollisionObjectContext_guess *objectContext;
    unsigned int *candidate;
    unsigned int *candidateEnd;
    CollisionInstance_guess *instance;
    Collision_guess *collision;
    MeshHolder_guess *meshHolder;
    AlternateMesh_guess *alternateMesh;
    CollisionWorldTransform_guess *worldTransform;
    int result;
    int hit;

    objectContext = ray->object_context;
    result = 0;
    candidate = collisionContext->candidate_list->indices;
    candidateEnd = candidate + collisionContext->candidate_list->count;
    if (candidate < candidateEnd)
    {
        do
        {
            instance = (CollisionInstance_guess *)objectContext->objects_by_index[*candidate];
            if (instance->query_state == EQGFX_COLLISION_COMPLETE_GUESS ||
                instance->query_state == EQGFX_COLLISION_BUSY)
                goto next_candidate;

            instance->query_state = EQGFX_COLLISION_BUSY;
            if (ray->sprite_callback != 0)
            {
                hit = ray->sprite_callback(instance, ray->callback_arg);
                if (hit == 0)
                {
                    instance->query_state = EQGFX_COLLISION_COMPLETE_GUESS;
                }
                else
                {
                    if (hit != 2)
                        goto test_collision;

                    hit = EQ_CDECL(int, 0x10006862, (Ray_guess *, float *, float))(
                        ray, instance->transform, instance->radius * instance->scale);
                    if (hit == 1)
                    {
                        ray->hit_object = 0;
                        ray->hit_sprite = instance;
                        result = 1;
                    }
                    else if (hit == 2)
                    {
                        instance->query_state = EQGFX_COLLISION_COMPLETE_GUESS;
                    }
                }
            }
            else
            {
test_collision:
                collision = (Collision_guess *)instance->collision_link->collision;
                if (collision != 0)
                {
                    if (collision->type == 0x48)
                    {
                        meshHolder = collision->mesh_holder;
                        EQGFX_GET_SPRITE_CENTER_GUESS(collision, scratch.center_offset);
                        if ((instance->flags & 0x10) == 0)
                        {
                            EQ_CDECL(void, 0x100046ED, (CollisionInstance_guess *, float *, float *))(
                                instance, scratch.center_offset, scratch.transformed_bounds);
                            if (meshHolder->mesh == 0)
                            {
                                alternateMesh = (AlternateMesh_guess *)meshHolder->alternate_mesh;
                                hit = EQ_CDECL(int, 0x10006916,
                                    (Ray_guess *, float *, float))(ray, scratch.transformed_bounds,
                                    alternateMesh->bounding_radius * instance->scale);
                            }
                            else
                            {
                                hit = EQ_CDECL(int, 0x10006916,
                                    (Ray_guess *, float *, float))(ray, scratch.transformed_bounds,
                                    meshHolder->mesh->bounding_radius * instance->scale);
                            }
                            if (hit != 1)
                                goto bounds_result;

                            EQ_CDECL(void, 0x10002B6B,
                                (CollisionInstance_guess *, float *, Ray_guess *, float *))(
                                instance, scratch.center_offset, ray, (float *)0x100AAD10);
                        }
                        else
                        {
                            EQ_CDECL(void, 0x10035058,
                                (CollisionInstance_guess *, float, float *))(
                                instance, instance->scale, instance->transform);
                            EQ_CDECL(void, 0x10002B6B,
                                (CollisionInstance_guess *, float *, Ray_guess *, float *))(
                                instance, scratch.center_offset, ray, (float *)0x100AAD10);
                            hit = EQ_CDECL(int, 0x100069C5,
                                (float *, Collision_guess *))((float *)0x100AAD10, collision);
                            if (hit != 0)
                                goto complete_candidate;
                        }

                        if (meshHolder->mesh != 0)
                        {
                            if (meshHolder->mesh->planes == 0 || collision->dynamic_sprite != 0)
                            {
                                hit = EQ_CDECL(int, 0x100053C0,
                                    (float *, Collision_guess *))((float *)0x100AAD10, collision);
                            }
                            else
                            {
                                hit = EQ_CDECL(int, 0x100051F9,
                                    (float *, Collision_guess *))((float *)0x100AAD10, collision);
                            }
                            goto collision_result;
                        }

                        if (meshHolder->alternate_mesh != 0)
                        {
                            alternateMesh = (AlternateMesh_guess *)meshHolder->alternate_mesh;
                            if (alternateMesh->collision_tree == 0)
                            {
                                if (collision->dynamic_sprite != 0)
                                    goto test_dynamic_mesh;
                                hit = EQ_CDECL(int, 0x10005CEE,
                                    (float *, Collision_guess *))((float *)0x100AAD10, collision);
                            }
                            else if (collision->dynamic_sprite == 0)
                            {
                                hit = EQ_CDECL(int, 0x10005A77,
                                    (float *, MeshHolder_guess *, SurfaceList_guess *))(
                                    (float *)0x100AAD10, meshHolder, collision->surface_list);
                            }
                            else
                            {
test_dynamic_mesh:
                                hit = EQ_CDECL(int, 0x100060EB,
                                    (float *, Collision_guess *))((float *)0x100AAD10, collision);
                            }
collision_result:
                            if (hit != 0)
                            {
                                ray->hit_sprite = instance;
                                ray->best_distance = EQ_GLOBAL(float, 0x100AAD60);
                                ray->hit_object = collision;
                                result = 1;
                                ray->hit_entry_number = EQ_GLOBAL(int, 0x100AAD6C);
                            }
                        }
                    }
                    else if (collision->type == 0x14)
                    {
                        hit = EQ_CDECL(int, 0x10006916,
                            (Ray_guess *, float *, float))(ray, instance->transform,
                            instance->radius * instance->scale);
                        if (hit == 1)
                        {
                            if ((instance->flags & 0x10) != 0)
                            {
                                EQ_CDECL(void, 0x1001B263,
                                    (void *, float *))(collision, scratch.center_offset);
                                EQ_CDECL(void, 0x10035058,
                                    (CollisionInstance_guess *, float, float *))(
                                    instance, instance->scale, instance->transform);
                                EQ_CDECL(void, 0x10002B6B,
                                    (CollisionInstance_guess *, float *, Ray_guess *, float *))(
                                    instance, scratch.center_offset, ray, (float *)0x100AAD10);
                                hit = EQ_CDECL(int, 0x10006B34,
                                    (float *, void *))((float *)0x100AAD10,
                                    collision);
                                if (hit != 0)
                                    goto complete_candidate;
                            }
                            EQ_CDECL(void, 0x100327C3,
                                (void *, void *, CollisionInstance_guess *))(
                                collision, ray->object_context, instance);
                            hit = EQ_CDECL(int, 0x1000504E,
                                (Ray_guess *, CollisionInstance_guess *, HierarchicalSprite_guess *))(
                                ray, instance, (HierarchicalSprite_guess *)collision);
                            if (hit != 0)
                            {
                                result = 1;
                                ray->hit_sprite = instance;
                            }
                        }
                        else if (hit == 2)
                        {
                            goto complete_candidate;
                        }
                    }
                    else if (collision->type == 0x0a || collision->type == 0x08)
                    {
                        hit = EQ_CDECL(int, 0x10006862,
                            (Ray_guess *, float *, float))(ray, instance->transform,
                            instance->radius * instance->scale);
                        if (hit != 1)
                            goto bounds_result;

                        ray->hit_object = 0;
                        ray->hit_sprite = instance;
                        result = 1;
                    }
                }
            }
            goto next_candidate;

bounds_result:
            if (hit == 2)
                goto complete_candidate;
            goto next_candidate;

complete_candidate:
            instance->query_state = EQGFX_COLLISION_COMPLETE_GUESS;

next_candidate:
            candidate++;
        }
        while (candidate < candidateEnd);
    }

    collision = (Collision_guess *)collisionContext->result_record->collision;
    if (collision != 0)
    {
        if (collision->type == 0x48)
        {
            meshHolder = collision->mesh_holder;
            if (meshHolder->mesh != 0)
            {
                hit = EQ_CDECL(int, 0x100051F9,
                    (Ray_guess *, Collision_guess *))(ray, collision);
                if (hit == 0)
                    return result;
                ray->hit_sprite = collisionContext;
                ray->hit_object = collision;
                return 1;
            }

            scratch.saved_ray[0] = ray->start[0];
            scratch.saved_ray[1] = ray->start[1];
            scratch.saved_ray[2] = ray->start[2];
            scratch.saved_ray[3] = ray->end[0];
            scratch.saved_ray[4] = ray->end[1];
            scratch.saved_ray[5] = ray->end[2];
            alternateMesh = (AlternateMesh_guess *)meshHolder->alternate_mesh;
            ray->start[0] -= alternateMesh->translation[0];
            ray->start[1] -= alternateMesh->translation[1];
            ray->start[2] -= alternateMesh->translation[2];
            ray->end[0] -= alternateMesh->translation[0];
            ray->end[1] -= alternateMesh->translation[1];
            ray->end[2] -= alternateMesh->translation[2];
            hit = EQ_CDECL(int, 0x10005A77,
                (Ray_guess *, MeshHolder_guess *, SurfaceList_guess *))(
                ray, meshHolder, collision->surface_list);
        }
        else
        {
            worldTransform = collision->world_transform;
            scratch.saved_ray[0] = ray->start[0];
            scratch.saved_ray[1] = ray->start[1];
            scratch.saved_ray[2] = ray->start[2];
            scratch.saved_ray[3] = ray->end[0];
            scratch.saved_ray[4] = ray->end[1];
            scratch.saved_ray[5] = ray->end[2];
            ray->start[0] -= worldTransform->translation[0];
            ray->start[1] -= worldTransform->translation[1];
            ray->start[2] -= worldTransform->translation[2];
            ray->end[0] -= worldTransform->translation[0];
            ray->end[1] -= worldTransform->translation[1];
            ray->end[2] -= worldTransform->translation[2];
            hit = EQ_CDECL(int, 0x10005A77, (Ray_guess *, Collision_guess *, int))(
                ray, collision, worldTransform->mesh_index);
        }

        if (hit != 0)
        {
            result = 1;
            ray->hit_sprite = collisionContext;
            ray->hit_object = collision;
        }
        ray->start[0] = scratch.saved_ray[0];
        ray->start[1] = scratch.saved_ray[1];
        ray->start[2] = scratch.saved_ray[2];
        ray->end[0] = scratch.saved_ray[3];
        ray->end[1] = scratch.saved_ray[4];
        ray->end[2] = scratch.saved_ray[5];
    }
    return result;
}

// FUNCTION: EQGFX 0x1000504E
// Finds the nearest ray hit across a hierarchical object's parts and nested objects.
int __cdecl s3dCollideRaySegmentWithHSprite_guess(Ray_guess *ray, void *transformContext, HierarchicalSprite_guess *hierarchy)
{
    int result;
    unsigned int childIndex;
    Collision_guess *collision;
    MeshHolder_guess *meshHolder;
    void *savedCallbackContext;
    CollisionTreeEntry_guess *entry;
    CollisionTreeEntry_guess *entryEnd;
    Ray_guess localRay;
    int hit;

    result = 0;
    childIndex = 0;
    if (hierarchy->child_count != 0)
    {
        do
        {
            collision = hierarchy->children[childIndex];
            meshHolder = collision->mesh_holder;
            EQ_CDECL(void, 0x1003D238, (TextureTable_guess *, MeshHolder_guess *))(ray->texture_table, meshHolder);
            if (meshHolder->mesh == 0)
            {
                if (meshHolder->alternate_mesh != 0)
                {
                    EQ_CDECL(void, 0x100386E0, (Collision_guess *, void *, HierarchicalSprite_guess *, TextureTable_guess *))(collision, transformContext, hierarchy, ray->texture_table);
                    savedCallbackContext = ray->callback_context;
                    ray->callback_context = transformContext;
                    hit = EQ_CDECL(int, 0x1000650E, (Ray_guess *, Collision_guess *))(ray, collision);
                    if (hit != 0)
                    {
                        result = 1;
                        ray->hit_object = collision;
                    }
                    ray->callback_context = savedCallbackContext;
                }
            }
            else
            {
                EQ_CDECL(void, 0x10038651, (Collision_guess *, void *, HierarchicalSprite_guess *, TextureTable_guess *))(collision, transformContext, hierarchy, ray->texture_table);
                savedCallbackContext = ray->callback_context;
                ray->callback_context = transformContext;
                hit = EQ_CDECL(int, 0x10005734, (Ray_guess *, Collision_guess *))(ray, collision);
                if (hit != 0)
                {
                    result = 1;
                    ray->hit_object = collision;
                }
                ray->callback_context = savedCallbackContext;
            }
            childIndex++;
        }
        while (childIndex < hierarchy->child_count);
    }

    entry = (CollisionTreeEntry_guess *)hierarchy->entries;
    entryEnd = entry + hierarchy->entry_count;
    do
    {
        if (entryEnd <= entry)
        {
            return result;
        }
        collision = (Collision_guess *)entry->object;
        if (collision != 0)
        {
            if (collision->type == 0x48)
            {
                EQ_CDECL(void, 0x10002CC3, (CollisionTreeEntry_guess *, Collision_guess *, void *, Ray_guess *, Ray_guess *))(entry, collision, transformContext, ray, &localRay);
                meshHolder = collision->mesh_holder;
                if (meshHolder->mesh == 0)
                {
                    if (meshHolder->alternate_mesh == 0)
                        goto next_entry;
                    if (((AlternateMesh_guess *)meshHolder->alternate_mesh)->collision_tree == 0 || collision->dynamic_sprite != 0)
                    {
                        hit = EQ_CDECL(int, 0x10005CEE, (Ray_guess *, Collision_guess *))(&localRay, collision);
                    }
                    else
                    {
                        hit = EQ_CDECL(int, 0x10005A77, (Ray_guess *, MeshHolder_guess *, SurfaceList_guess *))(&localRay, meshHolder, collision->surface_list);
                    }
                    if (hit == 0)
                        goto next_entry;
                    ray->hit_object = collision;
                    ray->best_distance = localRay.best_distance;
                }
                else
                {
                    if (meshHolder->mesh->planes == 0 || collision->dynamic_sprite != 0)
                    {
                        hit = EQ_CDECL(int, 0x100053C0, (Ray_guess *, Collision_guess *))(&localRay, collision);
                        if (hit == 0)
                            goto next_entry;
                        ray->hit_object = collision;
                        ray->best_distance = localRay.best_distance;
                    }
                    else
                    {
                        hit = EQ_CDECL(int, 0x100051F9, (Ray_guess *, Collision_guess *))(&localRay, collision);
                        if (hit == 0)
                            goto next_entry;
                        ray->best_distance = localRay.best_distance;
                        ray->hit_object = entry;
                    }
                }
                ray->hit_entry_number = localRay.hit_entry_number;
            }
            else if (collision->type != 0x14 || s3dCollideRaySegmentWithHSprite_guess(ray, transformContext, (HierarchicalSprite_guess *)collision) == 0)
            {
                goto next_entry;
            }
            result = 1;
        }
next_entry:
        entry++;
    }
    while (1);
}

// FUNCTION: EQGFX 0x100051f9
// Finds the nearest allowed ray hit on a float-vertex mesh with stored collision planes.
// __cdecl (two stack arguments, callee pops nothing). 1 when the segment ray->start..ray->end crosses a
// triangle of the collision mesh nearer than ray->best_distance, the ray's callback allowed to veto an
// entry; the hit stores its distance and 1-based entry number into the ray. 0 when nothing was crossed.
int __cdecl s3dCollideRaySegmentWithPrecomputedMeshPlanes_guess(Ray_guess *ray, Collision_guess *collision)
{
    CollisionMesh_guess *mesh;
    TriEntry_guess *entry;
    TriEntry_guess *entry_end;
    float *plane;
    Surface_guess *surface;
    unsigned int texture;
    float start_distance;
    float end_distance;
    float hit_distance;
    float point[3];
    int entry_number;
    int found;

    found = 0;
    mesh = collision->mesh_holder->mesh;
    plane = mesh->planes;
    if (plane == (float *)0)
        return 0;
    entry = mesh->triangles;
    entry_end = entry + mesh->triangle_count;
    if (entry < entry_end)
    {
        entry_number = 1;
        do
        {
            if (ray->test_all == 1 || (entry->flags & 0x11) == 0)
            {
                surface = collision->surface_list->surfaces[entry->surface_index];
                texture = surface->texture;
                if ((texture & 0x80000000) != 0)
                    texture = ray->texture_table->textures[texture & 0x7fffffff];
                if (ray->callback != (void *)0 &&
                    ray->callback(ray->callback_context, texture, ray->callback_arg) == 0)
                {
                    plane += 4;
                    entry++;
                    entry_number++;
                    continue;
                }
                start_distance = ray->start[2] * plane[2] + ray->start[0] * plane[0] +
                                 ray->start[1] * plane[1] + plane[3];
                end_distance = ray->end[1] * plane[1] + ray->end[2] * plane[2] +
                               ray->end[0] * plane[0] + plane[3];
                if ((EQGFX_ZERO_F_GUESS < start_distance && end_distance < EQGFX_ZERO_F_GUESS) ||
                    (surface->double_sided == 1 && start_distance < EQGFX_ZERO_F_GUESS &&
                     EQGFX_ZERO_F_GUESS < end_distance))
                {
                    hit_distance = start_distance / (start_distance - end_distance);
                    if (hit_distance < ray->best_distance)
                    {
                        point[0] = hit_distance * ray->direction[0] + ray->start[0];
                        point[1] = hit_distance * ray->direction[1] + ray->start[1];
                        point[2] = hit_distance * ray->direction[2] + ray->start[2];
                        if (EQ_CDECL(int, 0x100043FD, (float *, unsigned char, float *, float *, float *,
                                                       float *))(point, entry->flags, plane,
                                                                 mesh->vertices + entry->vertex0 * 3,
                                                                 mesh->vertices + entry->vertex1 * 3,
                                                                 mesh->vertices + entry->vertex2 * 3) != 0)
                        {
                            found = 1;
                            ray->best_distance = hit_distance;
                            ray->hit_entry_number = entry_number;
                        }
                    }
                }
            }
            plane += 4;
            entry++;
            entry_number++;
        } while (entry < entry_end);
    }
    return found;
}

// FUNCTION: EQGFX 0x100053c0
// Finds the nearest allowed ray hit on a float-vertex mesh, using its current animation pose when present.
int __cdecl s3dCollideRaySegmentWithCurrentFrameMesh_guess(float *rayPointer, int collisionAddress)
{
    Ray_guess *ray;
    Collision_guess *collision;
    MeshHolder_guess *meshHolder;
    CollisionMesh_guess *mesh;
    DynamicSprite_guess *dynamicSprite;
    TriEntry_guess *entry;
    TriEntry_guess *entryEnd;
    Surface_guess *surface;
    float *vertices;
    float *vertex0;
    float *vertex1;
    float *vertex2;
    float edge01X;
    float edge01Y;
    float edge01Z;
    float edge12X;
    float edge12Y;
    float edge12Z;
    float normalX;
    float normalY;
    float normalZ;
    float planeOffset;
    float startDistance;
    float endDistance;
    float hitDistance;
    float pointX;
    float pointY;
    float pointZ;
    unsigned int texture;
    int found;

    ray = (Ray_guess *)rayPointer;
    collision = (Collision_guess *)collisionAddress;
    found = 0;
    meshHolder = collision->mesh_holder;
    if (collision->dynamic_sprite == (DynamicSprite_guess *)0)
    {
        vertices = meshHolder->mesh->vertices;
    }
    else
    {
        EQ_CDECL(void, 0x10010F45, (Collision_guess *, unsigned int))(collision, ray->texture_table->timing_value);
        dynamicSprite = collision->dynamic_sprite;
        vertices = dynamicSprite->mesh_data->frames->vertices_by_frame[dynamicSprite->frame_index];
    }
    mesh = meshHolder->mesh;
    entry = mesh->triangles;
    entryEnd = entry + mesh->triangle_count;
    if (entry < entryEnd)
    {
        rayPointer = (float *)1;
        do
        {
            if (ray->test_all == 1 || (entry->flags & 0x11) == 0)
            {
                surface = collision->surface_list->surfaces[entry->surface_index];
                texture = surface->texture;
                if ((texture & 0x80000000) != 0)
                {
                    texture = ray->texture_table->textures[texture & 0x7fffffff];
                }
                if (ray->callback != (void *)0 &&
                    ray->callback(ray->callback_context, texture, ray->callback_arg) == 0)
                {
                    entry++;
                    rayPointer = (float *)((int)rayPointer + 1);
                    continue;
                }
                vertex0 = vertices + entry->vertex0 * 3;
                vertex1 = vertices + entry->vertex1 * 3;
                vertex2 = vertices + entry->vertex2 * 3;
                edge01X = vertex1[0] - vertex0[0];
                edge01Y = vertex1[1] - vertex0[1];
                edge01Z = vertex1[2] - vertex0[2];
                edge12X = vertex2[0] - vertex1[0];
                edge12Y = vertex2[1] - vertex1[1];
                edge12Z = vertex2[2] - vertex1[2];
                normalX = edge12Y * edge01Z - edge12Z * edge01Y;
                normalY = -(edge12X * edge01Z - edge12Z * edge01X);
                normalZ = edge12X * edge01Y - edge12Y * edge01X;
                planeOffset = -(normalY * vertex0[1] + normalX * vertex0[0] + normalZ * vertex0[2]);
                startDistance = normalX * ray->start[0] + normalY * ray->start[1] + normalZ * ray->start[2] + planeOffset;
                endDistance = normalX * ray->end[0] + normalY * ray->end[1] + normalZ * ray->end[2] + planeOffset;
                if ((EQGFX_ZERO_F_GUESS < startDistance && endDistance < EQGFX_ZERO_F_GUESS) ||
                    (surface->double_sided == 1 && startDistance < EQGFX_ZERO_F_GUESS &&
                     EQGFX_ZERO_F_GUESS < endDistance))
                {
                    hitDistance = startDistance / (startDistance - endDistance);
                    if (hitDistance < ray->best_distance)
                    {
                        pointX = hitDistance * ray->direction[0] + ray->start[0];
                        pointY = hitDistance * ray->direction[1] + ray->start[1];
                        pointZ = hitDistance * ray->direction[2] + ray->start[2];
                        if ((EQGFX_ZERO_F_GUESS <
                             (pointX - vertex0[0]) * (normalZ * edge01Y - normalY * edge01Z) +
                             (pointY - vertex0[1]) * -(normalZ * edge01X - normalX * edge01Z) +
                             (pointZ - vertex0[2]) * (normalY * edge01X - normalX * edge01Y)) &&
                            (EQGFX_ZERO_F_GUESS <
                             (pointX - vertex1[0]) * (normalZ * edge12Y - normalY * edge12Z) +
                             (pointZ - vertex1[2]) * (normalY * edge12X - normalX * edge12Y) +
                             (pointY - vertex1[1]) * -(normalZ * edge12X - normalX * edge12Z)) &&
                            (EQGFX_ZERO_F_GUESS <
                             (pointY - vertex2[1]) *
                             -((vertex0[0] - vertex2[0]) * normalZ - (vertex0[2] - vertex2[2]) * normalX) +
                             (pointZ - vertex2[2]) *
                             ((vertex0[0] - vertex2[0]) * normalY - (vertex0[1] - vertex2[1]) * normalX) +
                             (pointX - vertex2[0]) *
                             ((vertex0[1] - vertex2[1]) * normalZ - (vertex0[2] - vertex2[2]) * normalY)))
                        {
                            found = 1;
                            ray->best_distance = hitDistance;
                            ray->hit_entry_number = (int)rayPointer;
                        }
                    }
                }
            }
            entry++;
            rayPointer = (float *)((int)rayPointer + 1);
        } while (entry < entryEnd);
    }
    return found;
}

// FUNCTION: EQGFX 0x10005734
// Finds the nearest allowed ray hit on mesh triangles positioned in the shared rendering context.
// __cdecl. 1 when the segment ray->start..ray->end crosses a collision-mesh triangle nearer than ray->best_distance,
// the plane built from the triangle's own vertices, the callback allowed to veto; a hit records its distance and 1-based entry number. 0 otherwise.
int __cdecl s3dCollideRaySegmentWithContextVertices_guess(Ray_guess *ray, Collision_guess *collision)
{
    float start_distance;
    TriEntry_guess *entry;
    int found;
    TriEntry_guess *entry_end;
    float normal[3];
    float edge1[3];
    float edge2[3];
    float point[3];
    float cross3[3];
    CollisionMesh_guess *mesh;
    Surface_guess *surface;
    unsigned int texture;
    float *vertices;
    float *vertex0;
    float *vertex1;
    float *vertex2;
    float offset;
    float end_distance;
    float hit_distance;
    float cross1x;
    float cross1y;
    float cross1z;
    float cross2x;
    float cross2y;
    float cross2z;
    int entry_number;

    found = 0;
    mesh = collision->mesh_holder->mesh;
    entry = mesh->triangles;
    entry_end = entry + mesh->triangle_count;
    if (entry < entry_end)
    {
        entry_number = 1;
        do
        {
            if (ray->test_all == 1 || (entry->flags & 0x11) == 0)
            {
                surface = collision->surface_list->surfaces[entry->surface_index];
                texture = surface->texture;
                if ((texture & 0x80000000) != 0)
                    texture = ray->texture_table->textures[texture & 0x7fffffff];
                if (ray->callback != (void *)0 &&
                    ray->callback(ray->callback_context, texture, ray->callback_arg) == 0)
                {
                    entry++;
                    entry_number++;
                    continue;
                }
                vertices = ray->texture_table->vertices;
                vertex0 = vertices + entry->vertex0 * 3;
                vertex1 = vertices + entry->vertex1 * 3;
                vertex2 = vertices + entry->vertex2 * 3;
                edge1[0] = vertex1[0] - vertex0[0];
                edge1[1] = vertex1[1] - vertex0[1];
                edge1[2] = vertex1[2] - vertex0[2];
                edge2[0] = vertex2[0] - vertex1[0];
                edge2[1] = vertex2[1] - vertex1[1];
                edge2[2] = vertex2[2] - vertex1[2];
                normal[0] = edge2[1] * edge1[2] - edge2[2] * edge1[1];
                normal[1] = -(edge2[0] * edge1[2] - edge2[2] * edge1[0]);
                normal[2] = edge2[0] * edge1[1] - edge2[1] * edge1[0];
                offset = -(normal[1] * vertex0[1] + normal[0] * vertex0[0] + normal[2] * vertex0[2]);
                start_distance = normal[2] * ray->start[2] + normal[1] * ray->start[1] +
                                 normal[0] * ray->start[0] + offset;
                end_distance = normal[2] * ray->end[2] + normal[1] * ray->end[1] +
                               normal[0] * ray->end[0] + offset;
                if ((EQGFX_ZERO_F_GUESS < start_distance && end_distance < EQGFX_ZERO_F_GUESS) ||
                    (surface->double_sided == 1 && start_distance < EQGFX_ZERO_F_GUESS &&
                     EQGFX_ZERO_F_GUESS < end_distance))
                {
                    hit_distance = start_distance / (start_distance - end_distance);
                    if (hit_distance < ray->best_distance)
                    {
                        point[0] = hit_distance * ray->direction[0] + ray->start[0];
                        point[1] = hit_distance * ray->direction[1] + ray->start[1];
                        point[2] = hit_distance * ray->direction[2] + ray->start[2];
                        cross1x = normal[2] * edge1[1] - normal[1] * edge1[2];
                        cross1y = -(normal[2] * edge1[0] - normal[0] * edge1[2]);
                        cross1z = normal[1] * edge1[0] - normal[0] * edge1[1];
                        cross2x = normal[2] * edge2[1] - normal[1] * edge2[2];
                        cross2y = -(normal[2] * edge2[0] - normal[0] * edge2[2]);
                        cross2z = normal[1] * edge2[0] - normal[0] * edge2[1];
                        cross3[0] = (vertex0[1] - vertex2[1]) * normal[2] -
                                    (vertex0[2] - vertex2[2]) * normal[1];
                        cross3[1] = -((vertex0[0] - vertex2[0]) * normal[2] -
                                      (vertex0[2] - vertex2[2]) * normal[0]);
                        cross3[2] = (vertex0[0] - vertex2[0]) * normal[1] -
                                    (vertex0[1] - vertex2[1]) * normal[0];
                        if ((point[1] - vertex0[1]) * cross1y + (point[0] - vertex0[0]) * cross1x +
                                (point[2] - vertex0[2]) * cross1z > EQGFX_ZERO_F_GUESS &&
                            (point[2] - vertex1[2]) * cross2z + (point[1] - vertex1[1]) * cross2y +
                                (point[0] - vertex1[0]) * cross2x > EQGFX_ZERO_F_GUESS &&
                            (point[2] - vertex2[2]) * cross3[2] + (point[1] - vertex2[1]) * cross3[1] +
                                (point[0] - vertex2[0]) * cross3[0] > EQGFX_ZERO_F_GUESS)
                        {
                            found = 1;
                            ray->best_distance = hit_distance;
                            ray->hit_entry_number = entry_number;
                        }
                    }
                }
            }
            entry++;
            entry_number++;
        } while (entry < entry_end);
    }
    return found;
}

// File-local stand-ins for the mesh the second argument reaches (its vertices, its triangles, the groups
// grouping consecutive triangles under one surface, and the planes). Nothing in include/ describes them, so
// the names and the fields between the ones the original touches are guesses.

typedef struct MeshVertex_guess
{
    short           x;                     // 0x00  the mesh's scale table turns these into world units
    short           y;                     // 0x02
    short           z;                     // 0x04
    unsigned char   unused_06[0x14 - 0x06];
} MeshVertex_guess;

typedef struct MeshTri_guess
{
    unsigned short  flags;                 // 0x00  bits 0/4 skip when not test_all; 4/8 pick the plane
    unsigned short  vertex0;               // 0x02  indices into the mesh's vertex array
    unsigned short  vertex1;               // 0x04
    unsigned short  vertex2;               // 0x06
} MeshTri_guess;

typedef struct MeshGroup_guess
{
    unsigned short  triangle_count;        // 0x00  consecutive triangles sharing one surface
    unsigned short  surface_index;         // 0x02
} MeshGroup_guess;

typedef struct Mesh_guess
{
    unsigned char   unused_00[0x08];
    unsigned short  triangle_count;        // 0x08  entries in the triangle array
    unsigned char   unused_0a[0x12 - 0x0a];
    unsigned short  scale_index;           // 0x12  index into the scale table at 0x100fb868
    MeshVertex_guess *vertices;            // 0x14  0x14 bytes per vertex, x/y/z first
    MeshTri_guess  *triangles;             // 0x18  eight bytes per triangle
    unsigned char   unused_1c[0x20 - 0x1c];
    MeshGroup_guess *groups;               // 0x20  four bytes per group
    unsigned char   unused_24[0x74 - 0x24];
    float          *planes;                // 0x74  one 4-float plane per triangle
} Mesh_guess;

typedef struct MeshOwner_guess
{
    unsigned char   unused_00[0x18];
    Mesh_guess     *mesh;                  // 0x18
} MeshOwner_guess;

// 0x100fb868 the scale table the mesh's short vertices are multiplied by (mesh->scale_index indexes it).
#define EQGFX_MESH_SCALE_F_GUESS    EQ_ARRAY(float, 0x100FB868)

// FUNCTION: EQGFX 0x10005a77
// Finds the nearest allowed ray hit from either side of a scaled integer-vertex mesh with stored planes.
// __cdecl (three stack arguments, callee pops nothing). 1 when the segment ray->start..ray->end crosses a
// triangle of the mesh reached from mesh_owner (+0x18) nearer than ray->best_distance, the ray's callback
// first allowed to veto the whole group of triangles sharing a surface; a hit records its distance and 1-based
// entry number in the ray. 0 when nothing was crossed.
int __cdecl s3dCollideRaySegmentWithScaledMesh_guess(Ray_guess *ray, MeshOwner_guess *mesh_owner, SurfaceList_guess *surface_list)
{
    Mesh_guess *mesh;
    MeshVertex_guess *vertices;
    MeshGroup_guess *group;
    MeshTri_guess *entry;
    MeshTri_guess *entry_end;
    Surface_guess *surface;
    float *plane;
    unsigned int texture;
    float scale;
    float start_distance;
    float end_distance;
    float hit_distance;
    float point[3];
    float vertex0[3];
    float vertex1[3];
    float vertex2[3];
    int entry_index;
    int count;
    int result;

    result = 0;
    mesh = mesh_owner->mesh;
    plane = mesh->planes;
    if (plane == (float *)0)
        return 0;
    count = mesh->triangle_count;
    vertices = mesh->vertices;
    group = mesh->groups;
    entry = mesh->triangles;
    entry_index = 0;
    entry_end = entry + count;
    scale = EQGFX_MESH_SCALE_F_GUESS[mesh->scale_index];
    if (entry < entry_end)
    {
        do
        {
            count = group->triangle_count;
            surface = surface_list->surfaces[group->surface_index];
            texture = surface->texture;
            if ((texture & 0x80000000) != 0)
                texture = ray->texture_table->textures[texture & 0x7fffffff];
            if (ray->callback == (void *)0 ||
                ray->callback(ray->callback_context, texture, ray->callback_arg) != 0)
            {
                while (count != 0)
                {
                    if (ray->test_all == 1 || (entry->flags & 0x11) == 0)
                    {
                        start_distance = plane[2] * ray->start[2] + ray->start[1] * plane[1] +
                                         ray->start[0] * plane[0] + plane[3];
                        end_distance = ray->end[2] * plane[2] + ray->end[1] * plane[1] +
                                       ray->end[0] * plane[0] + plane[3];
                        if ((start_distance < EQGFX_ZERO_F_GUESS) != (end_distance < EQGFX_ZERO_F_GUESS))
                        {
                            hit_distance = start_distance / (start_distance - end_distance);
                            if (hit_distance < ray->best_distance)
                            {
                                point[0] = hit_distance * ray->direction[0] + ray->start[0];
                                point[1] = hit_distance * ray->direction[1] + ray->start[1];
                                point[2] = hit_distance * ray->direction[2] + ray->start[2];
                                vertex0[0] = (float)vertices[entry->vertex0].x * scale;
                                vertex0[1] = (float)vertices[entry->vertex0].y * scale;
                                vertex0[2] = (float)vertices[entry->vertex0].z * scale;
                                vertex1[0] = (float)vertices[entry->vertex1].x * scale;
                                vertex1[1] = (float)vertices[entry->vertex1].y * scale;
                                vertex1[2] = (float)vertices[entry->vertex1].z * scale;
                                vertex2[0] = (float)vertices[entry->vertex2].x * scale;
                                vertex2[1] = (float)vertices[entry->vertex2].y * scale;
                                vertex2[2] = (float)vertices[entry->vertex2].z * scale;
                                if (EQ_CDECL(int, 0x100043FD, (float *, unsigned short, float *, float *,
                                                               float *, float *))(point, entry->flags, plane,
                                                                                  vertex0, vertex1,
                                                                                  vertex2) != 0)
                                {
                                    result = 1;
                                    ray->best_distance = hit_distance;
                                    ray->hit_entry_number = entry_index + 1;
                                }
                            }
                        }
                    }
                    entry++;
                    plane += 4;
                    entry_index++;
                    count--;
                }
                group++;
            }
            else
            {
                entry_index += count;
                plane += count * 4;
                group++;
                entry += count;
            }
        } while (entry < entry_end);
    }
    return result;
}

// The second argument's object: the surface list (+0x20) and the owner of the mesh (+0x34, whose own +0x18
// holds the mesh). Nothing in include/ describes it, so its name and the untouched fields are guesses.
typedef struct MeshWorld_guess
{
    unsigned char   unused_00[0x20];
    SurfaceList_guess *surface_list;       // 0x20
    DynamicSprite_guess *dynamic_sprite;    // 0x24  animated packed vertices for this mesh
    unsigned char   unused_28[0x34 - 0x28];
    MeshOwner_guess  *mesh_owner;          // 0x34  the mesh owner, its mesh at +0x18
} MeshWorld_guess;

// FUNCTION: EQGFX 0x10005cee
// Finds the nearest allowed ray hit on a scaled integer-vertex mesh without requiring stored planes.
// __cdecl (two stack arguments, callee pops nothing). 1 when the segment ray->start..ray->end crosses a
// triangle of world's mesh nearer than ray->best_distance, its plane recomputed from the scaled vertices.
int __cdecl checkRayTriangleIntersection_guess(Ray_guess *ray, MeshWorld_guess *world)
{
    Mesh_guess *mesh;
    MeshVertex_guess *vertices;
    MeshGroup_guess *group;
    MeshTri_guess *entry;
    MeshTri_guess *entry_end;
    Surface_guess *surface;
    float scale;
    int entry_index;
    int result;
    unsigned int texture;
    int count;
    float x0, y0, z0;
    float x1, y1, z1;
    float x2, y2, z2;
    float e1x, e1y, e1z;
    float e2x, e2y, e2z;
    float nx, ny, nz;
    float plane_offset;
    float start_distance;
    float end_distance;
    float hit_distance;
    float point_x, point_y, point_z;

    result = 0;
    entry_index = 0;
    mesh = world->mesh_owner->mesh;
    entry = mesh->triangles;
    vertices = mesh->vertices;
    entry_end = entry + mesh->triangle_count;
    group = mesh->groups;
    scale = EQGFX_MESH_SCALE_F_GUESS[mesh->scale_index];
    if (entry < entry_end)
    {
        do
        {
            count = group->triangle_count;
            surface = world->surface_list->surfaces[group->surface_index];
            texture = surface->texture;
            if ((texture & 0x80000000) != 0)
                texture = ray->texture_table->textures[texture & 0x7fffffff];
            if (ray->callback == (void *)0 ||
                ray->callback(ray->callback_context, texture, ray->callback_arg) != 0)
            {
                while (count != 0)
                {
                    if (ray->test_all == 1 || (entry->flags & 0x11) == 0)
                    {
                        x0 = (float)vertices[entry->vertex0].x * scale;
                        y0 = (float)vertices[entry->vertex0].y * scale;
                        z0 = (float)vertices[entry->vertex0].z * scale;
                        x1 = (float)vertices[entry->vertex1].x * scale;
                        y1 = (float)vertices[entry->vertex1].y * scale;
                        z1 = (float)vertices[entry->vertex1].z * scale;
                        x2 = (float)vertices[entry->vertex2].x * scale;
                        y2 = (float)vertices[entry->vertex2].y * scale;
                        z2 = (float)vertices[entry->vertex2].z * scale;
                        e1x = x1 - x0;
                        e1y = y1 - y0;
                        e1z = z1 - z0;
                        e2x = x2 - x1;
                        e2y = y2 - y1;
                        e2z = z2 - z1;
                        nx = e2y * e1z - e2z * e1y;
                        ny = -(e2x * e1z - e2z * e1x);
                        nz = e2x * e1y - e2y * e1x;
                        plane_offset = -(nz * z0 + ny * y0 + nx * x0);
                        start_distance = nz * ray->start[2] + ny * ray->start[1] + nx * ray->start[0] +
                                         plane_offset;
                        end_distance = nz * ray->end[2] + ny * ray->end[1] + nx * ray->end[0] +
                                       plane_offset;
                        if ((start_distance > EQGFX_ZERO_F_GUESS &&
                             end_distance < EQGFX_ZERO_F_GUESS) ||
                            (surface->double_sided == 1 && start_distance < EQGFX_ZERO_F_GUESS &&
                             end_distance > EQGFX_ZERO_F_GUESS))
                        {
                            hit_distance = start_distance / (start_distance - end_distance);
                            if (hit_distance < ray->best_distance)
                            {
                                point_x = hit_distance * ray->direction[0] + ray->start[0];
                                point_y = hit_distance * ray->direction[1] + ray->start[1];
                                point_z = hit_distance * ray->direction[2] + ray->start[2];
                                if (EQGFX_ZERO_F_GUESS <
                                        (point_z - z0) * (ny * e1x - nx * e1y) +
                                        (point_y - y0) * -(nz * e1x - nx * e1z) +
                                        (point_x - x0) * (nz * e1y - ny * e1z) &&
                                    EQGFX_ZERO_F_GUESS <
                                        (point_z - z1) * (ny * e2x - nx * e2y) +
                                        (point_y - y1) * -(nz * e2x - nx * e2z) +
                                        (point_x - x1) * (nz * e2y - ny * e2z) &&
                                    EQGFX_ZERO_F_GUESS <
                                        (point_z - z2) * ((x0 - x2) * ny - (y0 - y2) * nx) +
                                        (point_y - y2) * -((x0 - x2) * nz - (z0 - z2) * nx) +
                                        (point_x - x2) * ((y0 - y2) * nz - (z0 - z2) * ny))
                                {
                                    result = 1;
                                    ray->best_distance = hit_distance;
                                    ray->hit_entry_number = entry_index + 1;
                                }
                            }
                        }
                    }
                    entry++;
                    entry_index++;
                    count--;
                }
                group++;
            }
            else
            {
                entry_index += count;
                group++;
                entry += count;
            }
        } while (entry < entry_end);
    }
    return result;
}
// FUNCTION: EQGFX 0x10002F16
// Limits a moving sphere's travel at contact with another sphere and records the contact normal.
// __cdecl, returns 0, 1 or 2. 2 when the axis ray (+0x2c..+0x34) misses the sphere (centre +0x08, radius + 0x3c); 0 when the
// sphere is not ahead or root * +0x38 is not below the limit (+0x50); else 1, with the limit set to that product (0 if behind).
int __cdecl s3dCollideMovingSphereWithSphere_guess(Sphere_guess *sphere, float *point, float radius)
{
    float dx;
    float dy;
    float dz;
    float dot;
    float effective_radius;
    float discriminant;
    float root;
    float candidate;
    float scale;

    dx = sphere->x - point[0];
    dy = sphere->y - point[1];
    dz = sphere->z - point[2];
    dot = dx * sphere->dir_x + dz * sphere->dir_z + dy * sphere->dir_y;
    if (dot < EQGFX_ZERO_F_GUESS)
    {
        effective_radius = radius + sphere->radius_offset;
        discriminant = dot * dot - ((dz * dz + dy * dy + dx * dx) - effective_radius * effective_radius);
        if (discriminant > EQGFX_ZERO_F_GUESS)
        {
            root = -(EQ_CDECL(float, 0x1004EF68, (float))(discriminant) + dot);
            candidate = root * sphere->scale;
            if (candidate < sphere->limit)
            {
                if (candidate >= EQGFX_ZERO_F_GUESS)
                {
                    sphere->limit = candidate;
                    sphere->normal_x = root * sphere->dir_x + sphere->x - point[0];
                    sphere->normal_y = root * sphere->dir_y + sphere->y - point[1];
                    sphere->normal_z = root * sphere->dir_z + sphere->z - point[2];
                    scale = 1.0f / effective_radius;
                }
                else
                {
                    sphere->limit = 0.0f;
                    sphere->normal_x = sphere->x - point[0];
                    sphere->normal_y = sphere->y - point[1];
                    dz = sphere->z - point[2];
                    sphere->normal_z = dz;
                    scale = EQ_GLOBAL(ReciprocalSqrtFn_guess, 0x100FC77C)(sphere->normal_x * sphere->normal_x +
                                                                          (sphere->normal_y * sphere->normal_y) + dz * dz);
                }
                sphere->normal_x = scale * sphere->normal_x;
                sphere->normal_y = scale * sphere->normal_y;
                sphere->normal_z = scale * sphere->normal_z;
                return 1;
            }
        }
        else
        {
            return 2;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x1000306A
// Checks whether a swept sphere reaches a target's bounding sphere.
// __cdecl, returns 0, 1 or 2. 1 when the point is inside the sphere (centre +0x08, radius + 0x3c); else the ray from
// the point along the axis (+0x2c): 2 when it misses, 1 when the hit is within the limit, else 0. Overwrites radius.
int __cdecl s3dTestMovingSphereAgainstBoundingSphere_guess(Sphere_guess *sphere, float *point, float radius)
{
    float dx;
    float dy;
    float dz;
    float outside_sq;
    float discriminant;
    float half_chord;

    radius = radius + sphere->radius_offset;
    dx = sphere->x - point[0];
    dy = sphere->y - point[1];
    dz = sphere->z - point[2];
    outside_sq = dz * dz + dy * dy + dx * dx - radius * radius;
    if (outside_sq <= 0.0f)
        return 1;
    radius = dz * sphere->dir_z + dy * sphere->dir_y + dx * sphere->dir_x;
    if (radius < 0.0f)
    {
        discriminant = radius * radius - outside_sq;
        if (discriminant <= 0.0f)
            return 2;
        half_chord = EQ_CDECL(float, 0x1004EF68, (float))(discriminant);
        if (-(half_chord + radius) * sphere->scale < sphere->limit)
            return 1;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1000312f
// Finds moving-sphere contacts with a polygon's face or boundary edges.
// __cdecl swept-sphere test against a projected polygon; the helper calls and fallback edge walk retain their original order.
int __cdecl s3dCollideMovingSphereWithPolygon_guess(VectorState_guess *state, ProjectedPolygon_guess *polygon)
{
    float *polygon_normal;
    float normal_alignment;
    float current_plane_distance;
    float previous_plane_distance;
    float interpolation;
    float tolerance;
    float point[3];
    unsigned int vertex_end;
    unsigned int previous_vertex;
    unsigned int vertex_address;
    int result;
    int hit;

    result = 0;
    polygon_normal = &polygon->normal_x;
    normal_alignment = state->normal[2] * polygon->normal_z +
                       polygon->normal_x * state->normal[0] + polygon->normal_y * state->normal[1];
    if (normal_alignment < 0.0f)
    {
        current_plane_distance = polygon->normal_y * state->position[1] +
                                 state->position[2] * polygon->normal_z +
                                 state->position[0] * *polygon_normal + polygon->plane_offset;
        if (0.0f < current_plane_distance)
        {
            current_plane_distance = current_plane_distance - state->radius;
            previous_plane_distance = state->previous_position[0] * *polygon_normal +
                                      state->previous_position[2] * polygon->normal_z +
                                      state->previous_position[1] * polygon->normal_y +
                                      polygon->plane_offset - state->radius;
            if (previous_plane_distance < 0.0f)
            {
                if (current_plane_distance == previous_plane_distance)
                {
                    vertex_end = (unsigned int)polygon->vertices + polygon->vertex_count * 0x14;
                    previous_vertex = vertex_end - 0x14;
                    for (vertex_address = (unsigned int)polygon->vertices;
                         vertex_address < vertex_end;
                         vertex_address = vertex_address + 0x14)
                    {
                        hit = EQ_CDECL(int, 0x10003484,
                                       (VectorState_guess *, PolygonSegment_guess *, PolygonVertex_guess *))
                            (state, (PolygonSegment_guess *)previous_vertex, (PolygonVertex_guess *)vertex_address);
                        if (hit != 0)
                            result = 1;
                        previous_vertex = vertex_address;
                    }
                }
                else
                {
                    interpolation = current_plane_distance / (current_plane_distance - previous_plane_distance);
                    tolerance = state->field_38 * 0.0001f;
                    if (interpolation <= tolerance + state->field_50)
                    {
                        if (interpolation < 0.0f)
                        {
                            interpolation = 0.0f;
                            current_plane_distance = current_plane_distance + state->radius;
                            point[0] = state->position[0] - current_plane_distance * *polygon_normal;
                            point[1] = state->position[1] - current_plane_distance * polygon->normal_y;
                            point[2] = state->position[2] - current_plane_distance * polygon->normal_z;
                        }
                        else
                        {
                            point[0] = (interpolation * state->movement[0] + state->position[0]) -
                                       *polygon_normal * state->radius;
                            point[1] = (interpolation * state->movement[1] + state->position[1]) -
                                       polygon->normal_y * state->radius;
                            point[2] = (interpolation * state->movement[2] + state->position[2]) -
                                       state->radius * polygon->normal_z;
                        }
                        if ((interpolation < state->field_50 - tolerance) ||
                            (normal_alignment <= state->field_5c * state->normal[0] +
                             state->field_60 * state->normal[1] + state->field_64 * state->normal[2]))
                        {
                            hit = EQ_CDECL(int, 0x10003808, (float *, ProjectedPolygon_guess *))(point, polygon);
                            if (hit == 0)
                            {
                                vertex_end = (unsigned int)polygon->vertices + polygon->vertex_count * 0x14;
                                previous_vertex = vertex_end - 0x14;
                                for (vertex_address = (unsigned int)polygon->vertices;
                                     vertex_address < vertex_end;
                                     vertex_address = vertex_address + 0x14)
                                {
                                    hit = EQ_CDECL(int, 0x10003484,
                                                   (VectorState_guess *, PolygonSegment_guess *, PolygonVertex_guess *))
                                        (state, (PolygonSegment_guess *)previous_vertex,
                                         (PolygonVertex_guess *)vertex_address);
                                    if (hit != 0)
                                        result = 1;
                                    previous_vertex = vertex_address;
                                }
                            }
                            else
                            {
                                state->field_5c = *polygon_normal;
                                state->field_60 = *(polygon_normal + 1);
                                state->field_64 = *(polygon_normal + 2);
                                state->field_50 = interpolation;
                                result = 1;
                            }
                        }
                        else
                        {
                            result = 0;
                        }
                    }
                }
            }
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10003484
// Finds moving-sphere contacts along a finite polygon edge, with a fallback test at its start vertex.
int __cdecl s3dCollideMovingSphereWithEdge(VectorState_guess *state, PolygonSegment_guess *segment, PolygonVertex_guess *vertex)
{
    float direction_x;
    float direction_y;
    float direction_z;
    float cross_x;
    float cross_y;
    float cross_z;
    float cross_length_sq;
    float inverse_cross_length;
    float unit_cross_x;
    float unit_cross_y;
    float unit_cross_z;
    float *segment_start;
    float offset_x;
    float offset_y;
    float offset_z;
    float signed_offset;
    float side_x;
    float side_y;
    float side_z;
    float side_denominator;
    float half_chord;
    float segment_parameter;
    float candidate_distance;
    float hit_x;
    float hit_y;
    float hit_z;
    float projected_distance;
    float normal_projection;
    float normal_x;
    float normal_y;
    float normal_z;
    float normal_scale;
    int starts_before_origin;

    direction_x = (vertex->point[0] - segment->start[0]) * segment->direction_scale;
    direction_y = (vertex->point[1] - segment->start[1]) * segment->direction_scale;
    direction_z = (vertex->point[2] - segment->start[2]) * segment->direction_scale;
    cross_x = direction_z * state->normal[1] - direction_y * state->normal[2];
    cross_y = -(direction_z * state->normal[0] - direction_x * state->normal[2]);
    cross_z = direction_y * state->normal[0] - direction_x * state->normal[1];
    cross_length_sq = cross_x * cross_x + cross_y * cross_y + cross_z * cross_z;
    if (cross_length_sq > EQGFX_ZERO_F_GUESS)
    {
        inverse_cross_length = EQGFX_INV_SQRT_GUESS(cross_length_sq);
        unit_cross_x = inverse_cross_length * cross_x;
        unit_cross_y = inverse_cross_length * cross_y;
        unit_cross_z = inverse_cross_length * cross_z;
        segment_start = segment->start;
        offset_x = state->position[0] - segment_start[0];
        offset_y = state->position[1] - segment_start[1];
        offset_z = state->position[2] - segment_start[2];
        signed_offset = offset_x * unit_cross_x + offset_y * unit_cross_y + offset_z * unit_cross_z;
        if ((signed_offset < state->radius) && (-state->radius < signed_offset))
        {
            side_x = unit_cross_y * direction_z - unit_cross_z * direction_y;
            side_y = -(unit_cross_x * direction_z - unit_cross_z * direction_x);
            side_z = unit_cross_x * direction_y - unit_cross_y * direction_x;
            side_denominator = side_x * state->normal[0] + side_y * state->normal[1] + side_z * state->normal[2];
            if ((side_denominator < EQGFX_ZERO_F_GUESS) &&
                (EQGFX_ZERO_F_GUESS < (((side_x * state->position[0] + side_y * state->position[1] +
                                         side_z * state->position[2]) - side_x * segment_start[0]) -
                                        side_y * segment_start[1]) - side_z * segment_start[2]))
            {
                half_chord = EQ_CDECL(float, 0x1004EF68, (float))
                    (state->radius * state->radius - signed_offset * signed_offset);
                segment_parameter = half_chord / side_denominator +
                    -(((offset_y * direction_z - offset_z * direction_y) * unit_cross_x +
                       -(offset_x * direction_z - offset_z * direction_x) * unit_cross_y +
                       (offset_x * direction_y - offset_y * direction_x) * unit_cross_z) * inverse_cross_length);
                candidate_distance = segment_parameter * state->field_38;
                if (candidate_distance < state->field_50)
                {
                    starts_before_origin = candidate_distance < EQGFX_ZERO_F_GUESS;
                    if (starts_before_origin)
                    {
                        hit_y = state->position[1];
                        hit_z = state->position[2];
                        candidate_distance = EQGFX_ZERO_F_GUESS;
                        hit_x = state->position[0];
                    }
                    else
                    {
                        hit_x = segment_parameter * state->normal[0] + state->position[0];
                        hit_y = segment_parameter * state->normal[1] + state->position[1];
                        hit_z = segment_parameter * state->normal[2] + state->position[2];
                    }
                    segment_start = segment->start;
                    projected_distance = hit_x * direction_x + hit_y * direction_y + hit_z * direction_z +
                        -(direction_x * segment_start[0] + direction_y * segment_start[1] +
                          direction_z * segment_start[2]);
                    if ((EQGFX_ZERO_F_GUESS < projected_distance) &&
                        (projected_distance < segment->parameter_limit))
                    {
                        state->field_5c = hit_x - segment_start[0];
                        state->field_60 = hit_y - segment_start[1];
                        normal_z = hit_z - segment_start[2];
                        state->field_64 = normal_z;
                        normal_projection = direction_y * state->field_60 + direction_x * state->field_5c +
                            normal_z * direction_z;
                        normal_x = state->field_5c - normal_projection * direction_x;
                        state->field_5c = normal_x;
                        normal_y = state->field_60 - normal_projection * direction_y;
                        state->field_60 = normal_y;
                        normal_z = state->field_64 - normal_projection * direction_z;
                        state->field_64 = normal_z;
                        if (starts_before_origin)
                        {
                            normal_scale = EQGFX_INV_SQRT_GUESS(normal_x * normal_x + normal_y * normal_y +
                                                               normal_z * normal_z);
                            state->field_5c = normal_scale * state->field_5c;
                            state->field_60 = normal_scale * state->field_60;
                            normal_scale = normal_scale * state->field_64;
                        }
                        else
                        {
                            state->field_5c = normal_x * state->field_44;
                            state->field_60 = normal_y * state->field_44;
                            normal_scale = state->field_64 * state->field_44;
                        }
                        state->field_64 = normal_scale;
                        state->field_50 = candidate_distance;
                        return 1;
                    }
                }
            }
            return EQ_CDECL(int, 0x10003331, (VectorState_guess *, float *))(state, segment->start);
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x10003808
// Checks whether a candidate collision point falls within a polygon's projected outline.
// __cdecl, returns 1 when the point is inside every edge of the polygon, projected on the plane `projection` names
// (2: y/z, 3: x/z, else x/y). Edge cross products (cyclic: y*z, z*x, x*y) must stay <= 0 if the normal is > 0, else >= 0.
int __cdecl s3dCheckPointInProjectedPolygon_guess(float *point, ProjectedPolygon_guess *polygon)
{
    PolygonVertex_guess *vertex;
    PolygonVertex_guess *end;
    float *prev;
    float *current;

    vertex = polygon->vertices;
    end = vertex + polygon->vertex_count;
    prev = (end - 1)->point;
    if (polygon->projection == 2)
    {
        if (polygon->normal_x > 0.0f)
        {
            for (; vertex < end; vertex++)
            {
                current = vertex->point;
                if ((current[2] - prev[2]) * (point[1] - prev[1])
                    - (current[1] - prev[1]) * (point[2] - prev[2]) < 0.0f)
                    return 0;
                prev = current;
            }
        }
        else
        {
            for (; vertex < end; vertex++)
            {
                current = vertex->point;
                if ((prev[2] - current[2]) * (point[1] - prev[1])
                    - (prev[1] - current[1]) * (point[2] - prev[2]) < 0.0f)
                    return 0;
                prev = current;
            }
        }
    }
    else if (polygon->projection == 3)
    {
        if (polygon->normal_y > 0.0f)
        {
            for (; vertex < end; vertex++)
            {
                current = vertex->point;
                if ((point[2] - prev[2]) * (current[0] - prev[0])
                    - (current[2] - prev[2]) * (point[0] - prev[0]) < 0.0f)
                    return 0;
                prev = current;
            }
        }
        else
        {
            for (; vertex < end; vertex++)
            {
                current = vertex->point;
                if ((point[2] - prev[2]) * (prev[0] - current[0])
                    - (prev[2] - current[2]) * (point[0] - prev[0]) < 0.0f)
                    return 0;
                prev = current;
            }
        }
    }
    else
    {
        if (polygon->normal_z > 0.0f)
        {
            for (; vertex < end; vertex++)
            {
                current = vertex->point;
                if ((current[1] - prev[1]) * (point[0] - prev[0])
                    - (point[1] - prev[1]) * (current[0] - prev[0]) < 0.0f)
                    return 0;
                prev = current;
            }
        }
        else
        {
            for (; vertex < end; vertex++)
            {
                current = vertex->point;
                if ((prev[1] - current[1]) * (point[0] - prev[0])
                    - (point[1] - prev[1]) * (prev[0] - current[0]) < 0.0f)
                    return 0;
                prev = current;
            }
        }
    }
    return 1;
}

// FUNCTION: EQGFX 0x10003ca3
// __cdecl: tests a sphere sweep against one mesh triangle and records the nearest accepted contact.
int __cdecl s3dCollideMovingSphereWithScaledMeshTriangle_guess(Sphere_guess *sphere, MeshOwner_guess *meshOwner, int triangleIndex)
{
    int result;
    Mesh_guess *mesh;
    MeshTri_guess *triangle;
    MeshVertex_guess *sourceVertex;
    float *plane;
    float centerDistance;
    float previousDistance;
    float tolerance;
    float intersection;
    float scale;
    float intersectionPoint[3];
    float vertex2[3];
    float vertex0[3];
    float vertex1[3];
    int callResult;

    result = 0;
    mesh = meshOwner->mesh;
    triangle = mesh->triangles + triangleIndex;
    plane = mesh->planes + triangleIndex * 4;
    if ((triangle->flags & 0x10) != 0)
        return 0;
    if (EQGFX_ZERO_F_GUESS <=
        (plane[1] * sphere->dir_y + plane[0] * sphere->dir_x + sphere->dir_z * plane[2]))
        return 0;
    if ((plane[0] * sphere->x + sphere->z * plane[2] + plane[1] * sphere->y + plane[3]) <=
        EQGFX_ZERO_F_GUESS)
        return 0;

    centerDistance = plane[0] * sphere->x + sphere->z * plane[2] + plane[1] * sphere->y + plane[3] -
        sphere->radius_offset;
    previousDistance = sphere->previous_y * plane[1] + sphere->previous_x * plane[0] +
        sphere->previous_z * plane[2] + plane[3] - sphere->radius_offset;
    if (previousDistance >= EQGFX_ZERO_F_GUESS)
        return 0;

    if (centerDistance == previousDistance)
    {
        scale = EQGFX_MESH_SCALE_F_GUESS[mesh->scale_index];
        sourceVertex = mesh->vertices + triangle->vertex0;
        vertex0[0] = (float)(int)sourceVertex->x * scale;
        vertex0[1] = (float)(int)sourceVertex->y * scale;
        vertex0[2] = (float)(int)sourceVertex->z * scale;
        sourceVertex = mesh->vertices + triangle->vertex1;
        vertex1[0] = (float)(int)sourceVertex->x * scale;
        vertex1[1] = (float)(int)sourceVertex->y * scale;
        vertex1[2] = (float)(int)sourceVertex->z * scale;
        sourceVertex = mesh->vertices + triangle->vertex2;
        vertex2[0] = (float)(int)sourceVertex->x * scale;
        vertex2[1] = (float)(int)sourceVertex->y * scale;
        vertex2[2] = (float)(int)sourceVertex->z * scale;
        callResult = EQ_CDECL(int, 0x1000405B, (Sphere_guess *, float *, float *))
            (sphere, vertex0, vertex1);
        if (callResult != 0)
            result = 1;
    }
    else
    {
        intersection = centerDistance / (centerDistance - previousDistance);
        tolerance = sphere->scale * EQGFX_SPAWN_AXIS_EPS_GUESS;
        if (tolerance + sphere->limit < intersection)
            return 0;
        if (intersection < EQGFX_ZERO_F_GUESS)
        {
            intersection = 0.0f;
            centerDistance = centerDistance + sphere->radius_offset;
            intersectionPoint[0] = sphere->x - centerDistance * plane[0];
            intersectionPoint[1] = sphere->y - centerDistance * plane[1];
            intersectionPoint[2] = sphere->z - centerDistance * plane[2];
        }
        else
        {
            intersectionPoint[0] = intersection * sphere->sweep_x + sphere->x - plane[0] * sphere->radius_offset;
            intersectionPoint[1] = intersection * sphere->sweep_y + sphere->y - plane[1] * sphere->radius_offset;
            intersectionPoint[2] = intersection * sphere->sweep_z + sphere->z - sphere->radius_offset * plane[2];
        }
        if ((sphere->limit - tolerance <= intersection) &&
            (sphere->normal_z * sphere->dir_z + sphere->normal_y * sphere->dir_y + sphere->normal_x * sphere->dir_x <
             sphere->dir_z * plane[2] + plane[0] * sphere->dir_x + plane[1] * sphere->dir_y))
            return 0;

        scale = EQGFX_MESH_SCALE_F_GUESS[mesh->scale_index];
        sourceVertex = mesh->vertices + triangle->vertex0;
        vertex0[0] = (float)(int)sourceVertex->x * scale;
        vertex0[1] = (float)(int)sourceVertex->y * scale;
        vertex0[2] = (float)(int)sourceVertex->z * scale;
        sourceVertex = mesh->vertices + triangle->vertex1;
        vertex1[0] = (float)(int)sourceVertex->x * scale;
        vertex1[1] = (float)(int)sourceVertex->y * scale;
        vertex1[2] = (float)(int)sourceVertex->z * scale;
        sourceVertex = mesh->vertices + triangle->vertex2;
        vertex2[0] = (float)(int)sourceVertex->x * scale;
        vertex2[1] = (float)(int)sourceVertex->y * scale;
        vertex2[2] = (float)(int)sourceVertex->z * scale;
        callResult = EQ_CDECL(int, 0x100043FD, (float *, unsigned short, float *, float *, float *, float *))
            (intersectionPoint, triangle->flags, plane, vertex0, vertex1, vertex2);
        if (callResult != 0)
        {
            sphere->normal_x = plane[0];
            sphere->normal_y = plane[1];
            sphere->normal_z = plane[2];
            sphere->limit = intersection;
            return 1;
        }
        callResult = EQ_CDECL(int, 0x1000405B, (Sphere_guess *, float *, float *))
            (sphere, vertex0, vertex1);
        if (callResult != 0)
            result = 1;
    }

    callResult = EQ_CDECL(int, 0x1000405B, (Sphere_guess *, float *, float *))
        (sphere, vertex1, vertex2);
    if (callResult != 0)
        result = 1;
    callResult = EQ_CDECL(int, 0x1000405B, (Sphere_guess *, float *, float *))
        (sphere, vertex2, vertex0);
    if (callResult == 0)
        return result;
    return 1;
}

// FUNCTION: EQGFX 0x100039f7
// __cdecl: tests a sphere sweep against one collision triangle and records an accepted contact.
int __cdecl s3dCollideMovingSphereWithFloatMeshTriangle_guess(Sphere_guess *sphere, MeshHolder_guess *meshHolder, int triangleIndex)
{
    int result;
    CollisionMesh_guess *mesh;
    TriEntry_guess *triangle;
    float *plane;
    float centerDistance;
    float previousDistance;
    float tolerance;
    float intersection;
    float intersectionPoint[3];
    float *vertex0;
    float *vertex1;
    float *vertex2;
    int callResult;

    result = 0;
    mesh = meshHolder->mesh;
    triangle = mesh->triangles + triangleIndex;
    plane = mesh->planes + triangleIndex * 4;
    if ((triangle->flags & 0x10) == 0)
    {
        if ((plane[1] * sphere->dir_y + plane[0] * sphere->dir_x + plane[2] * sphere->dir_z < EQGFX_ZERO_F_GUESS) &&
            (plane[1] * sphere->y + sphere->z * plane[2] + plane[0] * sphere->x + plane[3] > EQGFX_ZERO_F_GUESS))
        {
            centerDistance = plane[1] * sphere->y + sphere->z * plane[2] + plane[0] * sphere->x + plane[3] - sphere->radius_offset;
            previousDistance = sphere->previous_y * plane[1] + sphere->previous_x * plane[0] + sphere->previous_z * plane[2] + plane[3] - sphere->radius_offset;
            if (previousDistance < EQGFX_ZERO_F_GUESS)
            {
                if (centerDistance == previousDistance)
                {
                    vertex0 = mesh->vertices + triangle->vertex0 * 3;
                    vertex1 = mesh->vertices + triangle->vertex1 * 3;
                    vertex2 = mesh->vertices + triangle->vertex2 * 3;
                    callResult = EQ_CDECL(int, 0x1000405B, (Sphere_guess*, float*, float*))(sphere, vertex0, vertex1);
                    if (callResult != 0)
                        result = 1;
                    callResult = EQ_CDECL(int, 0x1000405B, (Sphere_guess*, float*, float*))(sphere, vertex1, vertex2);
                    if (callResult != 0)
                        result = 1;
                    callResult = EQ_CDECL(int, 0x1000405B, (Sphere_guess*, float*, float*))(sphere, vertex2, vertex0);
                    if (callResult == 0)
                        return result;
                }
                else
                {
                    intersection = centerDistance / (centerDistance - previousDistance);
                    tolerance = sphere->scale * EQGFX_SPAWN_AXIS_EPS_GUESS;
                    if (tolerance + sphere->limit < intersection)
                        return 0;
                    if (intersection < EQGFX_ZERO_F_GUESS)
                    {
                        intersection = 0.0f;
                        centerDistance = centerDistance + sphere->radius_offset;
                        intersectionPoint[0] = sphere->x - centerDistance * plane[0];
                        intersectionPoint[1] = sphere->y - centerDistance * plane[1];
                        intersectionPoint[2] = sphere->z - centerDistance * plane[2];
                    }
                    else
                    {
                        intersectionPoint[0] = intersection * sphere->sweep_x + sphere->x - plane[0] * sphere->radius_offset;
                        intersectionPoint[1] = intersection * sphere->sweep_y + sphere->y - plane[1] * sphere->radius_offset;
                        intersectionPoint[2] = intersection * sphere->sweep_z + sphere->z - sphere->radius_offset * plane[2];
                    }
                    if ((sphere->limit - tolerance <= intersection) &&
                        (sphere->normal_z * sphere->dir_z + sphere->normal_y * sphere->dir_y + sphere->normal_x * sphere->dir_x <
                         sphere->dir_z * plane[2] + plane[0] * sphere->dir_x + plane[1] * sphere->dir_y))
                        return 0;
                    vertex0 = mesh->vertices + triangle->vertex0 * 3;
                    vertex1 = mesh->vertices + triangle->vertex1 * 3;
                    vertex2 = mesh->vertices + triangle->vertex2 * 3;
                    callResult = EQ_CDECL(int, 0x100043FD, (float*, unsigned short, float*, float*, float*, float*))(
                        intersectionPoint, triangle->flags, plane, vertex0, vertex1, vertex2);
                    if (callResult == 0)
                    {
                        callResult = EQ_CDECL(int, 0x1000405B, (Sphere_guess*, float*, float*))(sphere, vertex0, vertex1);
                        if (callResult != 0)
                            result = 1;
                        callResult = EQ_CDECL(int, 0x1000405B, (Sphere_guess*, float*, float*))(sphere, vertex1, vertex2);
                        if (callResult != 0)
                            result = 1;
                        callResult = EQ_CDECL(int, 0x1000405B, (Sphere_guess*, float*, float*))(sphere, vertex2, vertex0);
                        if (callResult == 0)
                            return result;
                        return 1;
                    }
                    sphere->normal_x = plane[0];
                    sphere->normal_y = plane[1];
                    sphere->normal_z = plane[2];
                    sphere->limit = intersection;
                }
                result = 1;
            }
        }
    }
    else
    {
        result = 0;
    }
    return result;
}

// FUNCTION: EQGFX 0x1000405b
// __cdecl: tests a finite segment against the sphere's clipping plane and updates its nearest-hit normal.
int __cdecl s3dCollideMovingSphereWithDMEdge(Sphere_guess *sphere, float *segment_start, float *segment_end)
{
    float delta_x;
    float delta_y;
    float delta_z;
    float normal_x;
    float normal_y;
    float normal_z;
    float normal_length_squared;
    float inverse_normal_length;
    float relative_x;
    float relative_y;
    float relative_z;
    float plane_distance;
    float segment_length_squared;
    float inverse_segment_length;
    float segment_length;
    float cross_x;
    float cross_y;
    float cross_z;
    float plane_side;
    float candidate_parameter;
    float candidate_distance;
    float hit_x;
    float hit_y;
    float hit_z;
    float segment_projection;
    float normal_projection;
    float normal_scale;
    int candidate_is_negative;

    delta_x = *segment_end - *segment_start;
    delta_y = segment_end[1] - segment_start[1];
    delta_z = segment_end[2] - segment_start[2];
    normal_x = delta_z * sphere->dir_y - delta_y * sphere->dir_z;
    normal_y = -(delta_z * sphere->dir_x - delta_x * sphere->dir_z);
    normal_z = delta_y * sphere->dir_x - delta_x * sphere->dir_y;
    normal_length_squared = normal_x * normal_x + normal_y * normal_y + normal_z * normal_z;
    if (normal_length_squared > EQGFX_ZERO_F_GUESS)
    {
        inverse_normal_length = EQGFX_INV_SQRT_GUESS(normal_length_squared);
        normal_x = inverse_normal_length * normal_x;
        normal_y = inverse_normal_length * normal_y;
        normal_z = inverse_normal_length * normal_z;
        relative_x = sphere->x - *segment_start;
        relative_y = sphere->y - segment_start[1];
        relative_z = sphere->z - segment_start[2];
        plane_distance = relative_x * normal_x + relative_y * normal_y + relative_z * normal_z;
        if (plane_distance < sphere->radius_offset && -sphere->radius_offset < plane_distance)
        {
            segment_length_squared = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
            inverse_segment_length = EQGFX_INV_SQRT_GUESS(segment_length_squared);
            delta_x = inverse_segment_length * delta_x;
            delta_y = inverse_segment_length * delta_y;
            delta_z = inverse_segment_length * delta_z;
            segment_length = 1.0f / inverse_segment_length;
            cross_x = normal_y * delta_z - normal_z * delta_y;
            cross_y = -(normal_x * delta_z - normal_z * delta_x);
            cross_z = normal_x * delta_y - normal_y * delta_x;
            plane_side = cross_x * sphere->dir_x + cross_y * sphere->dir_y + cross_z * sphere->dir_z;
            if (plane_side < EQGFX_ZERO_F_GUESS &&
                EQGFX_ZERO_F_GUESS < (((cross_x * sphere->x + cross_y * sphere->y + cross_z * sphere->z -
                                        cross_x * *segment_start) - cross_y * segment_start[1]) -
                                      cross_z * segment_start[2]))
            {
                candidate_parameter = EQ_CDECL(float, 0x1004EF68, (float))
                    (sphere->radius_offset * sphere->radius_offset - plane_distance * plane_distance);
                candidate_parameter = candidate_parameter / plane_side -
                    (((relative_y * delta_z - relative_z * delta_y) * normal_x +
                      -(relative_x * delta_z - relative_z * delta_x) * normal_y +
                      (relative_x * delta_y - relative_y * delta_x) * normal_z) *
                     (1.0f / inverse_segment_length) * inverse_normal_length);
                candidate_distance = candidate_parameter * sphere->scale;
                if (candidate_distance < sphere->limit)
                {
                    candidate_is_negative = candidate_distance < EQGFX_ZERO_F_GUESS;
                    if (candidate_is_negative)
                    {
                        hit_x = sphere->x;
                        hit_y = sphere->y;
                        hit_z = sphere->z;
                        candidate_distance = 0.0f;
                    }
                    else
                    {
                        hit_x = candidate_parameter * sphere->dir_x + sphere->x;
                        hit_y = candidate_parameter * sphere->dir_y + sphere->y;
                        hit_z = candidate_parameter * sphere->dir_z + sphere->z;
                    }
                    segment_projection = hit_x * delta_x + hit_y * delta_y + hit_z * delta_z +
                        -(delta_x * *segment_start + delta_z * segment_start[2] + delta_y * segment_start[1]);
                    if (segment_projection > EQGFX_ZERO_F_GUESS && segment_projection < segment_length)
                    {
                        sphere->normal_x = hit_x - *segment_start;
                        sphere->normal_y = hit_y - segment_start[1];
                        sphere->normal_z = hit_z - segment_start[2];
                        normal_projection = delta_x * sphere->normal_x +
                            sphere->normal_z * delta_z + sphere->normal_y * delta_y;
                        normal_x = sphere->normal_x - normal_projection * delta_x;
                        sphere->normal_x = normal_x;
                        normal_y = sphere->normal_y - normal_projection * delta_y;
                        sphere->normal_y = normal_y;
                        normal_z = sphere->normal_z - normal_projection * delta_z;
                        sphere->normal_z = normal_z;
                        if (candidate_is_negative)
                        {
                            normal_scale = EQGFX_INV_SQRT_GUESS(normal_x * normal_x + normal_y * normal_y + normal_z * normal_z);
                            sphere->normal_x = normal_scale * sphere->normal_x;
                            sphere->normal_y = normal_scale * sphere->normal_y;
                            sphere->normal_z = normal_scale * sphere->normal_z;
                        }
                        else
                        {
                            sphere->normal_x = normal_x * sphere->normal_scale;
                            sphere->normal_y = normal_y * sphere->normal_scale;
                            sphere->normal_z = sphere->normal_z * sphere->normal_scale;
                        }
                        sphere->limit = candidate_distance;
                        return 1;
                    }
                }
            }
            return EQ_CDECL(int, 0x10003331, (Sphere_guess *, float *))(sphere, segment_start);
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x100060eb
// Finds the nearest ray contact with an animated mesh at its current frame, honoring surface filters.
int __cdecl checkRayTriangleMeshIntersection_guess(Ray_guess *ray, MeshWorld_guess *world)
{
    MeshOwner_guess *meshOwner;
    Mesh_guess *mesh;
    DynamicSprite_guess *dynamicSprite;
    MeshVertexStream_guess *vertexStream;
    short *vertexData;
    MeshTri_guess *triangle;
    MeshTri_guess *triangleEnd;
    MeshGroup_guess *group;
    Surface_guess *surface;
    float scale;
    int triangleIndex;
    int result;
    unsigned int texture;
    unsigned int triangleCount;
    float vertex0X;
    float vertex0Y;
    float vertex0Z;
    float vertex1X;
    float vertex1Y;
    float vertex1Z;
    float vertex2X;
    float vertex2Y;
    float vertex2Z;
    float edge01X;
    float edge01Y;
    float edge01Z;
    float edge12X;
    float edge12Y;
    float edge12Z;
    float normalX;
    float normalY;
    float normalZ;
    float planeOffset;
    float startDistance;
    float endDistance;
    float hitDistance;
    float hitX;
    float hitY;
    float hitZ;

    result = 0;
    triangleIndex = 0;
    meshOwner = world->mesh_owner;
    EQ_CDECL(void, 0x10010F45, (MeshWorld_guess *, unsigned int))
        (world, ray->texture_table->timing_value);
    dynamicSprite = world->dynamic_sprite;
    vertexStream = dynamicSprite->mesh_data->vertex_stream;
    vertexData = vertexStream->vertices +
        dynamicSprite->frame_index * vertexStream->vertex_count * 3;
    mesh = meshOwner->mesh;
    triangle = mesh->triangles;
    triangleEnd = triangle + mesh->triangle_count;
    group = mesh->groups;
    scale = EQGFX_MESH_SCALE_F_GUESS[mesh->scale_index];
    while (triangle < triangleEnd)
    {
        triangleCount = group->triangle_count;
        surface = world->surface_list->surfaces[group->surface_index];
        texture = surface->texture;
        if ((texture & 0x80000000) != 0)
            texture = ray->texture_table->textures[texture & 0x7fffffff];
        if (ray->callback == (int (__cdecl *)(void *, unsigned int, void *))0 ||
            ray->callback(ray->callback_context, texture, ray->callback_arg) != 0)
        {
            for (; triangleCount != 0; triangleCount--)
            {
                if (ray->test_all == 1 || (triangle->flags & 0x11) == 0)
                {
                    vertex0X = (float)vertexData[triangle->vertex0 * 3] * scale;
                    vertex0Y = (float)vertexData[triangle->vertex0 * 3 + 1] * scale;
                    vertex0Z = (float)vertexData[triangle->vertex0 * 3 + 2] * scale;
                    vertex1X = (float)vertexData[triangle->vertex1 * 3] * scale;
                    vertex1Y = (float)vertexData[triangle->vertex1 * 3 + 1] * scale;
                    vertex1Z = (float)vertexData[triangle->vertex1 * 3 + 2] * scale;
                    vertex2X = (float)vertexData[triangle->vertex2 * 3] * scale;
                    vertex2Y = (float)vertexData[triangle->vertex2 * 3 + 1] * scale;
                    vertex2Z = (float)vertexData[triangle->vertex2 * 3 + 2] * scale;
                    edge01X = vertex1X - vertex0X;
                    edge01Y = vertex1Y - vertex0Y;
                    edge01Z = vertex1Z - vertex0Z;
                    edge12X = vertex2X - vertex1X;
                    edge12Y = vertex2Y - vertex1Y;
                    edge12Z = vertex2Z - vertex1Z;
                    normalX = edge12Y * edge01Z - edge12Z * edge01Y;
                    normalY = -(edge12X * edge01Z - edge12Z * edge01X);
                    normalZ = edge12X * edge01Y - edge12Y * edge01X;
                    planeOffset = -(normalX * vertex0X + normalY * vertex0Y + normalZ * vertex0Z);
                    startDistance = normalX * ray->start[0] + normalY * ray->start[1] +
                        normalZ * ray->start[2] + planeOffset;
                    endDistance = normalX * ray->end[0] + normalY * ray->end[1] +
                        normalZ * ray->end[2] + planeOffset;
                    if ((((EQGFX_ZERO_F_GUESS < startDistance) && (endDistance < EQGFX_ZERO_F_GUESS)) ||
                         ((surface->double_sided == 1 && (startDistance < EQGFX_ZERO_F_GUESS)) &&
                          (EQGFX_ZERO_F_GUESS < endDistance))) &&
                        (hitDistance = startDistance / (startDistance - endDistance),
                         hitDistance < ray->best_distance))
                    {
                        hitX = hitDistance * ray->direction[0] + ray->start[0];
                        hitY = hitDistance * ray->direction[1] + ray->start[1];
                        hitZ = hitDistance * ray->direction[2] + ray->start[2];
                        if ((EQGFX_ZERO_F_GUESS <
                             (hitX - vertex0X) * (normalZ * edge01Y - normalY * edge01Z) +
                             (hitY - vertex0Y) * -(normalZ * edge01X - normalX * edge01Z) +
                             (hitZ - vertex0Z) * (normalY * edge01X - normalX * edge01Y)) &&
                            (EQGFX_ZERO_F_GUESS <
                             (hitX - vertex1X) * (normalZ * edge12Y - normalY * edge12Z) +
                             (hitY - vertex1Y) * -(normalZ * edge12X - normalX * edge12Z) +
                             (hitZ - vertex1Z) * (normalY * edge12X - normalX * edge12Y)) &&
                            (EQGFX_ZERO_F_GUESS <
                             (hitX - vertex2X) * ((vertex0Y - vertex2Y) * normalZ -
                                                   (vertex0Z - vertex2Z) * normalY) +
                             (hitY - vertex2Y) * -((vertex0X - vertex2X) * normalZ -
                                                   (vertex0Z - vertex2Z) * normalX) +
                             (hitZ - vertex2Z) * ((vertex0X - vertex2X) * normalY -
                                                   (vertex0Y - vertex2Y) * normalX)))
                        {
                            result = 1;
                            ray->best_distance = hitDistance;
                            ray->hit_entry_number = triangleIndex + 1;
                        }
                    }
                }
                triangle++;
                triangleIndex++;
            }
        }
        else
        {
            triangleIndex += triangleCount;
            triangle += triangleCount;
        }
        group++;
    }
    return result;
}
