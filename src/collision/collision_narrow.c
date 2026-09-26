/* Collision narrow phase functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* CollisionNarrow_AABBvsAABB
 * Tests intersection between two AABBs.
 * Args: r0=ax, r1=ay, r2=aw, r3=ah, sp+0=bx, sp+4=by, sp+8=bw, sp+0x0c=bh
 * Returns: 1 if intersecting, 0 otherwise */
s32 CollisionNarrow_AABBvsAABB(s32 ax, s32 ay, s32 aw, s32 ah,
                                 s32 bx, s32 by, s32 bw, s32 bh) {
    if (ax < bx + bw && ax + aw > bx &&
        ay < by + bh && ay + ah > by) {
        return 1;
    }
    return 0;
}

/* CollisionNarrow_AABBvsCircle
 * Tests intersection between an AABB and a circle.
 * Args: r0=ax, r1=ay, r2=aw, r3=ah, sp+0=cx, sp+4=cy, sp+8=radius
 * Returns: 1 if intersecting, 0 otherwise */
s32 CollisionNarrow_AABBvsCircle(s32 ax, s32 ay, s32 aw, s32 ah,
                                   s32 cx, s32 cy, s32 radius) {
    /* Find closest point on AABB to circle center */
    s32 closest_x = cx;
    s32 closest_y = cy;

    if (cx < ax) closest_x = ax;
    else if (cx > ax + aw) closest_x = ax + aw;

    if (cy < ay) closest_y = ay;
    else if (cy > ay + ah) closest_y = ay + ah;

    /* Check distance from closest point to circle center */
    s32 dx = cx - closest_x;
    s32 dy = cy - closest_y;

    return (dx * dx + dy * dy) <= (radius * radius) ? 1 : 0;
}

/* CollisionNarrow_CirclevsCircle
 * Tests intersection between two circles.
 * Args: r0=c1x, r1=c1y, r2=r1, sp+0=c2x, sp+4=c2y, sp+8=r2
 * Returns: 1 if intersecting, 0 otherwise */
s32 CollisionNarrow_CirclevsCircle(s32 c1x, s32 c1y, s32 r1,
                                     s32 c2x, s32 c2y, s32 r2) {
    s32 dx = c2x - c1x;
    s32 dy = c2y - c1y;
    s32 dist_sq = dx * dx + dy * dy;
    s32 radius_sum = r1 + r2;

    return dist_sq <= (radius_sum * radius_sum) ? 1 : 0;
}

/* CollisionNarrow_PointInAABB
 * Tests if a point is inside an AABB.
 * Args: r0=px, r1=py, r2=ax, r3=ay, sp+0=aw, sp+4=ah
 * Returns: 1 if inside, 0 otherwise */
s32 CollisionNarrow_PointInAABB(s32 px, s32 py, s32 ax, s32 ay, s32 aw, s32 ah) {
    return (px >= ax && px <= ax + aw &&
            py >= ay && py <= ay + ah) ? 1 : 0;
}

/* CollisionNarrow_PointInCircle
 * Tests if a point is inside a circle.
 * Args: r0=px, r1=py, r2=cx, r3=cy, sp+0=radius
 * Returns: 1 if inside, 0 otherwise */
s32 CollisionNarrow_PointInCircle(s32 px, s32 py, s32 cx, s32 cy, s32 radius) {
    s32 dx = px - cx;
    s32 dy = py - cy;
    return (dx * dx + dy * dy) <= (radius * radius) ? 1 : 0;
}

/* CollisionNarrow_LinevsAABB
 * Tests intersection between a line segment and an AABB.
 * Args: r0=x1, r1=y1, r2=x2, r3=y2, sp+0=ax, sp+4=ay, sp+8=aw, sp+0x0c=ah
 * Returns: 1 if intersecting, 0 otherwise */
s32 CollisionNarrow_LinevsAABB(s32 x1, s32 y1, s32 x2, s32 y2,
                                 s32 ax, s32 ay, s32 aw, s32 ah) {
    /* Check if either endpoint is inside */
    if (CollisionNarrow_PointInAABB(x1, y1, ax, ay, aw, ah) ||
        CollisionNarrow_PointInAABB(x2, y2, ax, ay, aw, ah)) {
        return 1;
    }

    /* Check line against each edge of AABB */
    s32 dx = x2 - x1;
    s32 dy = y2 - y1;

    /* Parametric intersection test */
    s32 tmin = 0;
    s32 tmax = 0x10000; /* 1.0 in fixed point */

    /* X axis */
    if (dx != 0) {
        s32 t1 = ((ax - x1) << 16) / dx;
        s32 t2 = ((ax + aw - x1) << 16) / dx;
        if (t1 > t2) { s32 tmp = t1; t1 = t2; t2 = tmp; }
        if (t1 > tmin) tmin = t1;
        if (t2 < tmax) tmax = t2;
    } else {
        if (x1 < ax || x1 > ax + aw) return 0;
    }

    /* Y axis */
    if (dy != 0) {
        s32 t1 = ((ay - y1) << 16) / dy;
        s32 t2 = ((ay + ah - y1) << 16) / dy;
        if (t1 > t2) { s32 tmp = t1; t1 = t2; t2 = tmp; }
        if (t1 > tmin) tmin = t1;
        if (t2 < tmax) tmax = t2;
    } else {
        if (y1 < ay || y1 > ay + ah) return 0;
    }

    return (tmin <= tmax) ? 1 : 0;
}

/* CollisionNarrow_Raycast
 * Performs a raycast against an AABB.
 * Args: r0=ox, r1=oy, r2=dx, r3=dy, sp+0=ax, sp+4=ay, sp+8=aw, sp+0x0c=ah
 * Returns: parametric distance (0x10000 = 1.0), or -1 if no hit */
s32 CollisionNarrow_Raycast(s32 ox, s32 oy, s32 dx, s32 dy,
                              s32 ax, s32 ay, s32 aw, s32 ah) {
    s32 tmin = 0;
    s32 tmax = 0x7FFFFFFF;

    /* X axis */
    if (dx != 0) {
        s32 inv_dx = (1 << 16) / dx;
        s32 t1 = (ax - ox) * inv_dx;
        s32 t2 = (ax + aw - ox) * inv_dx;
        if (t1 > t2) { s32 tmp = t1; t1 = t2; t2 = tmp; }
        if (t1 > tmin) tmin = t1;
        if (t2 < tmax) tmax = t2;
    } else {
        if (ox < ax || ox > ax + aw) return -1;
    }

    /* Y axis */
    if (dy != 0) {
        s32 inv_dy = (1 << 16) / dy;
        s32 t1 = (ay - oy) * inv_dy;
        s32 t2 = (ay + ah - oy) * inv_dy;
        if (t1 > t2) { s32 tmp = t1; t1 = t2; t2 = tmp; }
        if (t1 > tmin) tmin = t1;
        if (t2 < tmax) tmax = t2;
    } else {
        if (oy < ay || oy > ay + ah) return -1;
    }

    if (tmin <= tmax && tmin >= 0) {
        return tmin;
    }
    return -1;
}

/* CollisionNarrow_LinevsCircle
 * Tests intersection between a line segment and a circle.
 * Args: r0=x1, r1=y1, r2=x2, r3=y2, sp+0=cx, sp+4=cy, sp+8=radius
 * Returns: 1 if intersecting, 0 otherwise */
s32 CollisionNarrow_LinevsCircle(s32 x1, s32 y1, s32 x2, s32 y2,
                                   s32 cx, s32 cy, s32 radius) {
    /* Vector from line start to circle center */
    s32 dx = cx - x1;
    s32 dy = cy - y1;
    s32 line_dx = x2 - x1;
    s32 line_dy = y2 - y1;
    s32 line_len_sq = line_dx * line_dx + line_dy * line_dy;

    if (line_len_sq == 0) {
        /* Degenerate line (point) */
        return (dx * dx + dy * dy) <= (radius * radius) ? 1 : 0;
    }

    /* Project circle center onto line */
    s32 t = (dx * line_dx + dy * line_dy) / line_len_sq;

    /* Clamp to line segment */
    if (t < 0) t = 0;
    if (t > (1 << 16)) t = (1 << 16);

    /* Closest point on line */
    s32 closest_x = x1 + (line_dx * t >> 16);
    s32 closest_y = y1 + (line_dy * t >> 16);

    /* Distance from closest point to circle center */
    s32 dist_x = cx - closest_x;
    s32 dist_y = cy - closest_y;

    return (dist_x * dist_x + dist_y * dist_y) <= (radius * radius) ? 1 : 0;
}

/* CollisionNarrow_GetContact
 * Gets the contact point between two colliding shapes.
 * Args: r0=shape_a, r1=shape_b, r2=out_contact
 * Returns: 1 if contact found, 0 otherwise */
s32 CollisionNarrow_GetContact(void *shape_a, void *shape_b, void *out_contact) {
    s32 *a = (s32 *)shape_a;
    s32 *b = (s32 *)shape_b;
    s32 *out = (s32 *)out_contact;

    if (out == NULL) return 0;

    /* Assume both shapes are AABBs */
    s32 ax = a[0], ay = a[1], aw = a[2], ah = a[3];
    s32 bx = b[0], by = b[1], bw = b[2], bh = b[3];

    /* Find overlap */
    s32 overlap_x = (aw < bw ? aw : bw) - (ax < bx ? bx - ax : ax - bx);
    s32 overlap_y = (ah < bh ? ah : bh) - (ay < by ? by - ay : ay - by);

    if (overlap_x <= 0 || overlap_y <= 0) return 0;

    /* Contact point is center of overlap region */
    s32 cx = (ax + aw / 2 + bx + bw / 2) / 2;
    s32 cy = (ay + ah / 2 + by + bh / 2) / 2;

    /* Contact normal is minimum penetration axis */
    s32 nx = 0, ny = 0;
    if (overlap_x < overlap_y) {
        nx = (cx < (bx + bw / 2)) ? -1 : 1;
    } else {
        ny = (cy < (by + bh / 2)) ? -1 : 1;
    }

    out[0] = cx;  /* contact x */
    out[1] = cy;  /* contact y */
    out[2] = nx;  /* normal x */
    out[3] = ny;  /* normal y */
    out[4] = overlap_x < overlap_y ? overlap_x : overlap_y; /* penetration */

    return 1;
}
