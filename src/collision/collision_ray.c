/* Collision ray functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* Ray structure: {ox, oy, dx, dy, length} */
#define RAY_MAX 16

typedef struct {
    s32 ox;      /* +0x00: origin x */
    s32 oy;      /* +0x04: origin y */
    s32 dx;      /* +0x08: direction x */
    s32 dy;      /* +0x0c: direction y */
    s32 length;  /* +0x10: length */
} CollisionRay;

static CollisionRay collision_rays[RAY_MAX];
static u32 ray_count = 0;

/* CollisionRay_Init
 * Initializes a ray with origin and direction.
 * Args: r0=ray_index, r1=ox, r2=oy, r3=dx, sp+0=dy, sp+4=length */
void CollisionRay_Init(u32 ray_index, s32 ox, s32 oy, s32 dx, s32 dy, s32 length) {
    if (ray_index < RAY_MAX) {
        collision_rays[ray_index].ox = ox;
        collision_rays[ray_index].oy = oy;
        collision_rays[ray_index].dx = dx;
        collision_rays[ray_index].dy = dy;
        collision_rays[ray_index].length = length;
    }
}

/* CollisionRay_Set
 * Sets a ray component.
 * Args: r0=ray_index, r1=offset, r2=value */
void CollisionRay_Set(u32 ray_index, u32 offset, u32 value) {
    if (ray_index < RAY_MAX) {
        u32 *data = (u32 *)&collision_rays[ray_index];
        data[offset / 4] = value;
    }
}

/* CollisionRay_Get
 * Gets a ray component.
 * Args: r0=ray_index, r1=offset
 * Returns: component value */
u32 CollisionRay_Get(u32 ray_index, u32 offset) {
    if (ray_index < RAY_MAX) {
        u32 *data = (u32 *)&collision_rays[ray_index];
        return data[offset / 4];
    }
    return 0;
}

/* CollisionRay_SetLength
 * Sets the ray length.
 * Args: r0=ray_index, r1=length */
void CollisionRay_SetLength(u32 ray_index, s32 length) {
    if (ray_index < RAY_MAX) {
        collision_rays[ray_index].length = length;
    }
}

/* CollisionRay_GetLength
 * Gets the ray length.
 * Args: r0=ray_index
 * Returns: length */
s32 CollisionRay_GetLength(u32 ray_index) {
    if (ray_index < RAY_MAX) {
        return collision_rays[ray_index].length;
    }
    return 0;
}

/* CollisionRay_Normalize
 * Normalizes the ray direction to unit length.
 * Args: r0=ray_index */
void CollisionRay_Normalize(u32 ray_index) {
    if (ray_index >= RAY_MAX) return;

    CollisionRay *ray = &collision_rays[ray_index];
    s32 dx = ray->dx;
    s32 dy = ray->dy;

    /* Calculate length squared */
    u32 len_sq = (u32)(dx * dx + dy * dy);
    if (len_sq == 0) return;

    /* Fixed-point sqrt (8.8 format) */
    extern u32 Math_Sqrt(u32 val);
    u32 len = Math_Sqrt(len_sq);
    if (len == 0) return;

    /* Normalize to unit vector (8.8 fixed point) */
    ray->dx = (dx << 8) / (s32)len;
    ray->dy = (dy << 8) / (s32)len;
}

/* CollisionRay_GetPoint
 * Gets a point along the ray at parametric distance t.
 * Args: r0=ray_index, r1=t, r2=out_x, r3=out_y */
void CollisionRay_GetPoint(u32 ray_index, s32 t, s32 *out_x, s32 *out_y) {
    if (ray_index >= RAY_MAX) return;

    CollisionRay *ray = &collision_rays[ray_index];

    /* point = origin + t * direction (8.8 fixed point) */
    if (out_x != NULL) {
        *out_x = ray->ox + (ray->dx * t >> 8);
    }
    if (out_y != NULL) {
        *out_y = ray->oy + (ray->dy * t >> 8);
    }
}

/* CollisionRay_Intersect
 * Tests ray against another ray.
 * Args: r0=ray_a, r1=ray_b, r2=out_t
 * Returns: 1 if intersecting, 0 otherwise */
s32 CollisionRay_Intersect(u32 ray_a, u32 ray_b, s32 *out_t) {
    if (ray_a >= RAY_MAX || ray_b >= RAY_MAX) return 0;

    CollisionRay *ra = &collision_rays[ray_a];
    CollisionRay *rb = &collision_rays[ray_b];

    /* Compute denominator */
    s32 denom = ra->dx * rb->dy - ra->dy * rb->dx;
    if (denom == 0) return 0; /* Parallel rays */

    s32 ox = rb->ox - ra->ox;
    s32 oy = rb->oy - ra->oy;

    /* Compute t parameter */
    s32 t = (ox * rb->dy - oy * rb->dx) / denom;

    if (out_t != NULL) {
        *out_t = t;
    }

    /* Check if intersection is within ray length */
    return (t >= 0 && t <= ra->length) ? 1 : 0;
}

/* CollisionRay_IntersectAABB
 * Tests ray against an AABB.
 * Args: r0=ray_index, r1=ax, r2=ay, r3=aw, sp+0=ah, sp+4=out_t
 * Returns: 1 if intersecting, 0 otherwise */
s32 CollisionRay_IntersectAABB(u32 ray_index, s32 ax, s32 ay, s32 aw, s32 ah,
                                s32 *out_t) {
    if (ray_index >= RAY_MAX) return 0;

    CollisionRay *ray = &collision_rays[ray_index];

    /* Use parametric AABB intersection */
    s32 tmin = 0;
    s32 tmax = ray->length;

    /* X axis */
    if (ray->dx != 0) {
        s32 inv_dx = (1 << 16) / ray->dx;
        s32 t1 = (ax - ray->ox) * inv_dx;
        s32 t2 = (ax + aw - ray->ox) * inv_dx;
        if (t1 > t2) { s32 tmp = t1; t1 = t2; t2 = tmp; }
        if (t1 > tmin) tmin = t1;
        if (t2 < tmax) tmax = t2;
    } else {
        if (ray->ox < ax || ray->ox > ax + aw) return 0;
    }

    /* Y axis */
    if (ray->dy != 0) {
        s32 inv_dy = (1 << 16) / ray->dy;
        s32 t1 = (ay - ray->oy) * inv_dy;
        s32 t2 = (ay + ah - ray->oy) * inv_dy;
        if (t1 > t2) { s32 tmp = t1; t1 = t2; t2 = tmp; }
        if (t1 > tmin) tmin = t1;
        if (t2 < tmax) tmax = t2;
    } else {
        if (ray->oy < ay || ray->oy > ay + ah) return 0;
    }

    if (tmin <= tmax && tmin >= 0) {
        if (out_t != NULL) {
            *out_t = tmin;
        }
        return 1;
    }
    return 0;
}

/* CollisionRay_IntersectCircle
 * Tests ray against a circle.
 * Args: r0=ray_index, r1=cx, r2=cy, r3=radius, sp+0=out_t
 * Returns: 1 if intersecting, 0 otherwise */
s32 CollisionRay_IntersectCircle(u32 ray_index, s32 cx, s32 cy, s32 radius,
                                  s32 *out_t) {
    if (ray_index >= RAY_MAX) return 0;

    CollisionRay *ray = &collision_rays[ray_index];

    /* Vector from ray origin to circle center */
    s32 ocx = cx - ray->ox;
    s32 ocy = cy - ray->oy;

    /* Project onto ray direction */
    s32 t = (ocx * ray->dx + ocy * ray->dy) >> 8;

    /* Clamp to ray length */
    if (t < 0) t = 0;
    if (t > ray->length) t = ray->length;

    /* Closest point on ray */
    s32 closest_x = ray->ox + (ray->dx * t >> 8);
    s32 closest_y = ray->oy + (ray->dy * t >> 8);

    /* Distance from closest point to circle center */
    s32 dx = cx - closest_x;
    s32 dy = cy - closest_y;

    if (dx * dx + dy * dy <= radius * radius) {
        if (out_t != NULL) {
            *out_t = t;
        }
        return 1;
    }
    return 0;
}
