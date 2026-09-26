/* AABB collision and camera space functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* External globals */
extern s32 g_camera_x;
extern s32 g_camera_y;
extern u32 g_window_width;
extern u32 g_window_height;
extern u32 collision_entity_count;
extern void *collision_entity_list;

/* Collision_CameraSpaceLookup
 * Converts world-space coordinates to camera-space.
 * Args: r0=x, r1=y
 * Returns: packed camera-space result or -1 if offscreen */
u32 Collision_CameraSpaceLookup(s32 world_x, s32 world_y) {
    s32 cam_x = g_camera_x;
    s32 cam_y = g_camera_y;
    s32 screen_w = (s32)g_window_width;
    s32 screen_h = (s32)g_window_height;

    /* Convert to camera space */
    s32 cx = world_x - cam_x;
    s32 cy = world_y - cam_y;

    /* Check if onscreen (with some margin) */
    if (cx < -32 || cx > screen_w + 32 ||
        cy < -32 || cy > screen_h + 32) {
        return 0xFFFFFFFF; /* offscreen */
    }

    /* Pack coordinates into result (upper 16 = x, lower 16 = y) */
    return (u32)(((cx & 0xFFFF) << 16) | (cy & 0xFFFF));
}

/* Collision_AABB_Test
 * Tests intersection between two AABBs.
 * Args: r0=ax, r1=ay, r2=aw, r3=ah, sp+0=bx, sp+4=by, sp+8=bw, sp+0x0c=bh
 * Returns: 1 if intersecting, 0 otherwise */
s32 Collision_AABB_Test(s32 ax, s32 ay, s32 aw, s32 ah,
                         s32 bx, s32 by, s32 bw, s32 bh) {
    /* Standard AABB overlap test */
    if (ax < bx + bw && ax + aw > bx &&
        ay < by + bh && ay + ah > by) {
        return 1;
    }
    return 0;
}

/* Collision_EntityAABBSweep
 * Sweeps an AABB along a direction and tests against all collision entities.
 * Args: r0=entity, r1=x, r2=y, r3=width, sp+0=height, sp+4=direction
 * Returns: depth to first collision entity, or 0x18 if none */
s32 Collision_EntityAABBSweep(void *entity, s32 x, s32 y, s32 width, s32 height,
                               s32 direction) {
    u32 count = collision_entity_count;
    void **list = (void **)&collision_entity_list;
    s32 best_depth = 0x18; /* 24 pixels max */

    for (u32 i = 0; i < count; i++) {
        void *ent = list[i];
        if (ent == NULL) continue;

        void *data = *(void **)ent;
        if (data == NULL || data == entity) continue;

        /* Get entity AABB */
        s32 ex = *((s32 *)data + 4); /* +0x10: pos_x */
        s32 ey = *((s32 *)data + 5); /* +0x14: pos_y */
        u16 ew = *((u16 *)ent + 16); /* +0x20: width */
        u16 eh = *((u16 *)ent + 17); /* +0x22: height */

        if (ew == 0 || eh == 0) continue;

        /* Convert entity position to same space */
        s32 ent_x = ex >> 8;
        s32 ent_y = ey >> 8;
        s32 ent_w = (s32)ew;
        s32 ent_h = (s32)eh;

        /* Check AABB overlap */
        s32 dx = x - ent_x;
        s32 dy = y - ent_y;

        s32 overlap_x = (width + ent_w) - (dx < 0 ? -dx : dx);
        s32 overlap_y = (height + ent_h) - (dy < 0 ? -dy : dy);

        if (overlap_x > 0 && overlap_y > 0) {
            /* Compute penetration along sweep direction */
            s32 depth;
            switch (direction) {
                case 0: depth = overlap_x; break; /* right */
                case 1: depth = overlap_y; break; /* up */
                case 2: depth = overlap_x; break; /* left */
                case 3: depth = overlap_y; break; /* down */
                default: depth = overlap_x < overlap_y ? overlap_x : overlap_y; break;
            }

            if (depth < best_depth) {
                best_depth = depth;
            }
        }
    }

    return best_depth;
}
