/* Collision sweep functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* External math functions */
extern u32 Math_Sqrt(u32 val);

/* External globals */
extern u32 g_sweep_results;
extern u32 collision_entity_count;
extern void *collision_entity_list;

/* Sweep test constants */
#define SWEEP_MAX_RESULTS 32
#define SWEEP_MARGIN 24

/* Sweep result structure */
typedef struct {
    void *entity;      /* +0x00: colliding entity */
    s32 depth;         /* +0x04: penetration depth */
    s32 normal_x;      /* +0x08: contact normal x */
    s32 normal_y;      /* +0x0c: contact normal y */
    u32 active;        /* +0x10: active flag */
} SweepResult;

static SweepResult sweep_results[SWEEP_MAX_RESULTS];
static u32 sweep_count = 0;

/* CollisionSweep_Init
 * Initializes the sweep test system.
 * Args: none */
void CollisionSweep_Init(void) {
    sweep_count = 0;
    for (u32 i = 0; i < SWEEP_MAX_RESULTS; i++) {
        sweep_results[i].entity = NULL;
        sweep_results[i].depth = 0;
        sweep_results[i].normal_x = 0;
        sweep_results[i].normal_y = 0;
        sweep_results[i].active = 0;
    }
}

/* CollisionSweep_AABB
 * Sweeps an AABB against all collision entities.
 * Args: r0=ax, r1=ay, r2=aw, r3=ah, sp+0=dx, sp+4=dy
 * Returns: number of collisions found */
u32 CollisionSweep_AABB(s32 ax, s32 ay, s32 aw, s32 ah, s32 dx, s32 dy) {
    u32 count = 0;
    u32 entity_count = collision_entity_count;
    void **entity_list = (void **)&collision_entity_list;

    /* Sweep position = start + delta */
    s32 sweep_x = ax + dx;
    s32 sweep_y = ay + dy;

    for (u32 i = 0; i < entity_count && count < SWEEP_MAX_RESULTS; i++) {
        void *ent = entity_list[i];
        if (ent == NULL) continue;

        void *data = *(void **)ent;
        if (data == NULL) continue;

        /* Get entity AABB */
        s32 ex = *((s32 *)data + 4) >> 8; /* +0x10: pos_x */
        s32 ey = *((s32 *)data + 5) >> 8; /* +0x14: pos_y */
        u16 ew = *((u16 *)ent + 16);       /* +0x20: width */
        u16 eh = *((u16 *)ent + 17);       /* +0x22: height */

        if (ew == 0 || eh == 0) continue;

        /* Check if swept AABB overlaps entity */
        if (sweep_x < ex + (s32)ew && sweep_x + aw > ex &&
            sweep_y < ey + (s32)eh && sweep_y + ah > ey) {
            /* Calculate penetration depth */
            s32 pen_x, pen_y;
            s32 overlap_x = (aw + (s32)ew) - (sweep_x < ex ? ex - sweep_x : sweep_x - ex);
            s32 overlap_y = (ah + (s32)eh) - (sweep_y < ey ? ey - sweep_y : sweep_y - ey);

            s32 depth = overlap_x < overlap_y ? overlap_x : overlap_y;

            /* Calculate contact normal */
            s32 nx = 0, ny = 0;
            if (overlap_x < overlap_y) {
                nx = (sweep_x < ex) ? -1 : 1;
            } else {
                ny = (sweep_y < ey) ? -1 : 1;
            }

            /* Store result */
            sweep_results[count].entity = ent;
            sweep_results[count].depth = depth;
            sweep_results[count].normal_x = nx;
            sweep_results[count].normal_y = ny;
            sweep_results[count].active = 1;

            count++;
        }
    }

    sweep_count = count;
    return count;
}

/* CollisionSweep_Circle
 * Sweeps a circle against all collision entities.
 * Args: r0=cx, r1=cy, r2=radius, r3=dx, sp+0=dy
 * Returns: number of collisions found */
u32 CollisionSweep_Circle(s32 cx, s32 cy, s32 radius, s32 dx, s32 dy) {
    u32 count = 0;
    u32 entity_count = collision_entity_count;
    void **entity_list = (void **)&collision_entity_list;

    /* Sweep center */
    s32 sweep_x = cx + dx;
    s32 sweep_y = cy + dy;

    for (u32 i = 0; i < entity_count && count < SWEEP_MAX_RESULTS; i++) {
        void *ent = entity_list[i];
        if (ent == NULL) continue;

        void *data = *(void **)ent;
        if (data == NULL) continue;

        /* Get entity AABB */
        s32 ex = *((s32 *)data + 4) >> 8;
        s32 ey = *((s32 *)data + 5) >> 8;
        u16 ew = *((u16 *)ent + 16);
        u16 eh = *((u16 *)ent + 17);

        if (ew == 0 || eh == 0) continue;

        /* Find closest point on AABB to circle */
        s32 closest_x = sweep_x;
        s32 closest_y = sweep_y;

        if (sweep_x < ex) closest_x = ex;
        else if (sweep_x > ex + (s32)ew) closest_x = ex + (s32)ew;

        if (sweep_y < ey) closest_y = ey;
        else if (sweep_y > ey + (s32)eh) closest_y = ey + (s32)eh;

        /* Check distance */
        s32 dx2 = sweep_x - closest_x;
        s32 dy2 = sweep_y - closest_y;
        s32 dist_sq = dx2 * dx2 + dy2 * dy2;

        if (dist_sq <= radius * radius) {
            s32 depth = radius - (s32)Math_Sqrt((u32)dist_sq);

            s32 nx = 0, ny = 0;
            if (dist_sq > 0) {
                nx = (dx2 << 8) / (s32)Math_Sqrt((u32)dist_sq);
                ny = (dy2 << 8) / (s32)Math_Sqrt((u32)dist_sq);
            }

            sweep_results[count].entity = ent;
            sweep_results[count].depth = depth;
            sweep_results[count].normal_x = nx;
            sweep_results[count].normal_y = ny;
            sweep_results[count].active = 1;

            count++;
        }
    }

    sweep_count = count;
    return count;
}

/* CollisionSweep_TestAABB
 * Tests AABB sweep against a specific entity.
 * Args: r0=entity, r1=ax, r2=ay, r3=aw, sp+0=ah, sp+4=dx, sp+8=dy
 * Returns: penetration depth, or -1 if no collision */
s32 CollisionSweep_TestAABB(void *entity, s32 ax, s32 ay, s32 aw, s32 ah,
                             s32 dx, s32 dy) {
    if (entity == NULL) return -1;

    void *data = *(void **)entity;
    if (data == NULL) return -1;

    /* Get entity AABB */
    s32 ex = *((s32 *)data + 4) >> 8;
    s32 ey = *((s32 *)data + 5) >> 8;
    u16 ew = *((u16 *)entity + 16);
    u16 eh = *((u16 *)entity + 17);

    if (ew == 0 || eh == 0) return -1;

    /* Swept position */
    s32 sweep_x = ax + dx;
    s32 sweep_y = ay + dy;

    /* Check overlap */
    if (sweep_x < ex + (s32)ew && sweep_x + aw > ex &&
        sweep_y < ey + (s32)eh && sweep_y + ah > ey) {
        s32 overlap_x = (aw + (s32)ew) - (sweep_x < ex ? ex - sweep_x : sweep_x - ex);
        s32 overlap_y = (ah + (s32)eh) - (sweep_y < ey ? ey - sweep_y : sweep_y - ey);
        return overlap_x < overlap_y ? overlap_x : overlap_y;
    }

    return -1;
}

/* CollisionSweep_TestCircle
 * Tests circle sweep against a specific entity.
 * Args: r0=entity, r1=cx, r2=cy, r3=radius, sp+0=dx, sp+4=dy
 * Returns: penetration depth, or -1 if no collision */
s32 CollisionSweep_TestCircle(void *entity, s32 cx, s32 cy, s32 radius,
                               s32 dx, s32 dy) {
    if (entity == NULL) return -1;

    void *data = *(void **)entity;
    if (data == NULL) return -1;

    /* Get entity AABB */
    s32 ex = *((s32 *)data + 4) >> 8;
    s32 ey = *((s32 *)data + 5) >> 8;
    u16 ew = *((u16 *)entity + 16);
    u16 eh = *((u16 *)entity + 17);

    if (ew == 0 || eh == 0) return -1;

    /* Swept center */
    s32 sweep_x = cx + dx;
    s32 sweep_y = cy + dy;

    /* Find closest point on AABB to circle */
    s32 closest_x = sweep_x;
    s32 closest_y = sweep_y;

    if (sweep_x < ex) closest_x = ex;
    else if (sweep_x > ex + (s32)ew) closest_x = ex + (s32)ew;

    if (sweep_y < ey) closest_y = ey;
    else if (sweep_y > ey + (s32)eh) closest_y = ey + (s32)eh;

    /* Check distance */
    s32 dx2 = sweep_x - closest_x;
    s32 dy2 = sweep_y - closest_y;
    s32 dist_sq = dx2 * dx2 + dy2 * dy2;

    if (dist_sq <= radius * radius) {
        return radius - (s32)Math_Sqrt((u32)dist_sq);
    }

    return -1;
}

/* CollisionSweep_GetNormal
 * Gets the contact normal for a sweep result.
 * Args: r0=index, r1=out_x, r2=out_y */
void CollisionSweep_GetNormal(u32 index, s32 *out_x, s32 *out_y) {
    if (index < sweep_count && sweep_results[index].active) {
        if (out_x != NULL) *out_x = sweep_results[index].normal_x;
        if (out_y != NULL) *out_y = sweep_results[index].normal_y;
    } else {
        if (out_x != NULL) *out_x = 0;
        if (out_y != NULL) *out_y = 0;
    }
}

/* CollisionSweep_Clear
 * Clears all sweep results.
 * Args: none */
void CollisionSweep_Clear(void) {
    sweep_count = 0;
    for (u32 i = 0; i < SWEEP_MAX_RESULTS; i++) {
        sweep_results[i].active = 0;
    }
}
