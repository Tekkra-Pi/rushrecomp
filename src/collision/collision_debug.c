/* Collision debug and mask functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* External globals */
extern u32 g_collision_debug;
extern u32 g_layer_collision_matrix[];
extern u32 g_collision_pairs;

/* Maximum collision masks and layers */
#define COLL_MASK_MAX 32
#define COLL_LAYER_MAX 16

/* Collision mask storage */
static u32 collision_masks[COLL_MASK_MAX];
static u32 collision_mask_count = 0;

/* Collision layer storage */
typedef struct {
    s32 x;
    s32 y;
    u32 param;
    u32 active;
} CollLayerEntry;

static CollLayerEntry collision_layers[COLL_LAYER_MAX];
static u32 collision_layer_count = 0;

/* Debug draw state */
static u32 debug_draw_active = 0;
static u32 debug_draw_timer = 0;

/* CollisionDebug_Init
 * Initializes the collision debug system.
 * Args: none */
void CollisionDebug_Init(void) {
    debug_draw_active = 0;
    debug_draw_timer = 0;
    g_collision_debug = 0;
}

/* CollisionDebug_Draw
 * Draws collision debug visualization.
 * Args: none */
void CollisionDebug_Draw(void) {
    if (!debug_draw_active) return;

    /* Debug drawing would render collision boxes, normals, etc. */
    /* This is a stub for the debug visualization system */
}

/* CollisionDebug_Update
 * Updates collision debug state.
 * Args: none */
void CollisionDebug_Update(void) {
    if (debug_draw_timer > 0) {
        debug_draw_timer--;
        if (debug_draw_timer == 0) {
            debug_draw_active = 0;
        }
    }
}

/* CollisionMask_Init
 * Initializes the collision mask system.
 * Args: none */
void CollisionMask_Init(void) {
    collision_mask_count = 0;
    for (u32 i = 0; i < COLL_MASK_MAX; i++) {
        collision_masks[i] = 0;
    }
}

/* CollisionMask_Set
 * Sets a collision mask value.
 * Args: r0=index, r1=value */
void CollisionMask_Set(u32 index, u32 value) {
    if (index < COLL_MASK_MAX) {
        collision_masks[index] = value;
    }
}

/* CollisionMask_Check
 * Checks if two masks collide.
 * Args: r0=mask_a, r1=mask_b
 * Returns: 1 if masks collide, 0 otherwise */
s32 CollisionMask_Check(u32 mask_a, u32 mask_b) {
    /* Collision if any bits overlap */
    return (mask_a & mask_b) != 0 ? 1 : 0;
}

/* CollisionMask_Get
 * Gets a collision mask value.
 * Args: r0=index
 * Returns: mask value */
u32 CollisionMask_Get(u32 index) {
    if (index < COLL_MASK_MAX) {
        return collision_masks[index];
    }
    return 0;
}

/* CollisionLayer_Init
 * Initializes the collision layer system.
 * Args: none */
void CollisionLayer_Init(void) {
    collision_layer_count = 0;
    for (u32 i = 0; i < COLL_LAYER_MAX; i++) {
        collision_layers[i].x = 0;
        collision_layers[i].y = 0;
        collision_layers[i].param = 0;
        collision_layers[i].active = 0;
    }
}

/* CollisionLayer_Add
 * Adds an entry to the collision layer.
 * Args: r0=x, r1=y, r2=param */
void CollisionLayer_Add(s32 x, s32 y, u32 param) {
    if (collision_layer_count < COLL_LAYER_MAX) {
        collision_layers[collision_layer_count].x = x;
        collision_layers[collision_layer_count].y = y;
        collision_layers[collision_layer_count].param = param;
        collision_layers[collision_layer_count].active = 1;
        collision_layer_count++;
    }
}

/* CollisionLayer_Remove
 * Removes an entry from the collision layer.
 * Args: r0=index */
void CollisionLayer_Remove(u32 index) {
    if (index < collision_layer_count) {
        collision_layers[index].active = 0;
        /* Compact array */
        for (u32 i = index; i < collision_layer_count - 1; i++) {
            collision_layers[i] = collision_layers[i + 1];
        }
        collision_layer_count--;
    }
}

/* CollisionLayer_Check
 * Checks if a point collides with any layer entry.
 * Args: r0=x, r1=y
 * Returns: 1 if colliding, 0 otherwise */
s32 CollisionLayer_Check(s32 x, s32 y) {
    for (u32 i = 0; i < collision_layer_count; i++) {
        if (!collision_layers[i].active) continue;

        s32 lx = collision_layers[i].x;
        s32 ly = collision_layers[i].y;
        u32 param = collision_layers[i].param;

        /* Simple AABB check using param as size */
        s32 half_w = (s32)((param >> 16) & 0xFFFF);
        s32 half_h = (s32)(param & 0xFFFF);

        if (x >= lx - half_w && x <= lx + half_w &&
            y >= ly - half_h && y <= ly + half_h) {
            return 1;
        }
    }
    return 0;
}
