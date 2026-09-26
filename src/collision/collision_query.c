/* Collision query functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* External math functions */
extern u32 Math_Sqrt(u32 val);

/* External globals */
extern u32 g_query_max;
extern u32 g_query_results;
extern u32 collision_entity_count;
extern void *collision_entity_list;

/* Query constants */
#define QUERY_MAX_RESULTS 64

/* Query result structure */
typedef struct {
    void *entity;      /* +0x00: matching entity */
    s32 distance;      /* +0x04: distance to query point */
    u32 active;        /* +0x08: active flag */
} QueryResult;

static QueryResult query_results[QUERY_MAX_RESULTS];
static u32 query_count = 0;

/* CollisionQuery_Init
 * Initializes the query system.
 * Args: none */
void CollisionQuery_Init(void) {
    query_count = 0;
    for (u32 i = 0; i < QUERY_MAX_RESULTS; i++) {
        query_results[i].entity = NULL;
        query_results[i].distance = 0;
        query_results[i].active = 0;
    }
}

/* CollisionQuery_Area
 * Queries all entities within an AABB area.
 * Args: r0=ax, r1=ay, r2=aw, r3=ah
 * Returns: number of matching entities */
u32 CollisionQuery_Area(s32 ax, s32 ay, s32 aw, s32 ah) {
    query_count = 0;
    u32 entity_count = collision_entity_count;
    void **entity_list = (void **)&collision_entity_list;

    for (u32 i = 0; i < entity_count && query_count < QUERY_MAX_RESULTS; i++) {
        void *ent = entity_list[i];
        if (ent == NULL) continue;

        void *data = *(void **)ent;
        if (data == NULL) continue;

        /* Get entity position */
        s32 ex = *((s32 *)data + 4) >> 8;
        s32 ey = *((s32 *)data + 5) >> 8;
        u16 ew = *((u16 *)ent + 16);
        u16 eh = *((u16 *)ent + 17);

        /* Check AABB overlap */
        if (ax < ex + (s32)ew && ax + aw > ex &&
            ay < ey + (s32)eh && ay + ah > ey) {
            query_results[query_count].entity = ent;
            query_results[query_count].distance = 0;
            query_results[query_count].active = 1;
            query_count++;
        }
    }

    g_query_results = query_count;
    return query_count;
}

/* CollisionQuery_Point
 * Queries all entities containing a point.
 * Args: r0=px, r1=py
 * Returns: number of matching entities */
u32 CollisionQuery_Point(s32 px, s32 py) {
    query_count = 0;
    u32 entity_count = collision_entity_count;
    void **entity_list = (void **)&collision_entity_list;

    for (u32 i = 0; i < entity_count && query_count < QUERY_MAX_RESULTS; i++) {
        void *ent = entity_list[i];
        if (ent == NULL) continue;

        void *data = *(void **)ent;
        if (data == NULL) continue;

        /* Get entity position and size */
        s32 ex = *((s32 *)data + 4) >> 8;
        s32 ey = *((s32 *)data + 5) >> 8;
        u16 ew = *((u16 *)ent + 16);
        u16 eh = *((u16 *)ent + 17);

        /* Check point inside AABB */
        if (px >= ex && px <= ex + (s32)ew &&
            py >= ey && py <= ey + (s32)eh) {
            query_results[query_count].entity = ent;
            query_results[query_count].distance = 0;
            query_results[query_count].active = 1;
            query_count++;
        }
    }

    g_query_results = query_count;
    return query_count;
}

/* CollisionQuery_Ray
 * Queries all entities intersecting a ray.
 * Args: r0=ox, r1=oy, r2=dx, r3=dy
 * Returns: number of matching entities */
u32 CollisionQuery_Ray(s32 ox, s32 oy, s32 dx, s32 dy) {
    query_count = 0;
    u32 entity_count = collision_entity_count;
    void **entity_list = (void **)&collision_entity_list;

    for (u32 i = 0; i < entity_count && query_count < QUERY_MAX_RESULTS; i++) {
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

        /* Parametric ray-AABB test */
        s32 tmin = 0;
        s32 tmax = 0x7FFFFFFF;

        if (dx != 0) {
            s32 inv_dx = (1 << 16) / dx;
            s32 t1 = (ex - ox) * inv_dx;
            s32 t2 = (ex + (s32)ew - ox) * inv_dx;
            if (t1 > t2) { s32 tmp = t1; t1 = t2; t2 = tmp; }
            if (t1 > tmin) tmin = t1;
            if (t2 < tmax) tmax = t2;
        } else {
            if (ox < ex || ox > ex + (s32)ew) continue;
        }

        if (dy != 0) {
            s32 inv_dy = (1 << 16) / dy;
            s32 t1 = (ey - oy) * inv_dy;
            s32 t2 = (ey + (s32)eh - oy) * inv_dy;
            if (t1 > t2) { s32 tmp = t1; t1 = t2; t2 = tmp; }
            if (t1 > tmin) tmin = t1;
            if (t2 < tmax) tmax = t2;
        } else {
            if (oy < ey || oy > ey + (s32)eh) continue;
        }

        if (tmin <= tmax && tmin >= 0) {
            query_results[query_count].entity = ent;
            query_results[query_count].distance = tmin;
            query_results[query_count].active = 1;
            query_count++;
        }
    }

    g_query_results = query_count;
    return query_count;
}

/* CollisionQuery_Circle
 * Queries all entities intersecting a circle.
 * Args: r0=cx, r1=cy, r2=radius
 * Returns: number of matching entities */
u32 CollisionQuery_Circle(s32 cx, s32 cy, s32 radius) {
    query_count = 0;
    u32 entity_count = collision_entity_count;
    void **entity_list = (void **)&collision_entity_list;
    u32 radius_sq = (u32)(radius * radius);

    for (u32 i = 0; i < entity_count && query_count < QUERY_MAX_RESULTS; i++) {
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

        /* Find closest point on AABB to circle center */
        s32 closest_x = cx;
        s32 closest_y = cy;

        if (cx < ex) closest_x = ex;
        else if (cx > ex + (s32)ew) closest_x = ex + (s32)ew;

        if (cy < ey) closest_y = ey;
        else if (cy > ey + (s32)eh) closest_y = ey + (s32)eh;

        /* Check distance */
        s32 dx = cx - closest_x;
        s32 dy = cy - closest_y;
        u32 dist_sq = (u32)(dx * dx + dy * dy);

        if (dist_sq <= radius_sq) {
            query_results[query_count].entity = ent;
            query_results[query_count].distance = (s32)Math_Sqrt(dist_sq);
            query_results[query_count].active = 1;
            query_count++;
        }
    }

    g_query_results = query_count;
    return query_count;
}

/* CollisionQuery_GetCount
 * Returns the number of query results.
 * Args: none
 * Returns: result count */
u32 CollisionQuery_GetCount(void) {
    return query_count;
}

/* CollisionQuery_Clear
 * Clears all query results.
 * Args: none */
void CollisionQuery_Clear(void) {
    query_count = 0;
    for (u32 i = 0; i < QUERY_MAX_RESULTS; i++) {
        query_results[i].active = 0;
    }
}
