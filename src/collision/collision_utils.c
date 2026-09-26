/* Tile raycast and collision utility functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 80-116
 * tile_raycast @ 0x02066638 (1112 bytes)
 * sloped_surface_response @ 0x020670bc (1084 bytes) */

#include "nds_types.h"
#include "player.h"

/* External globals from stage collision system */
extern u32 g_stage_collision_width;
extern u32 g_stage_data_ptr;
extern s32 g_camera_x;
extern s32 g_camera_y;

/* External math functions */
extern u32 Math_Sqrt(u32 val);

/* Global collision entity data */
extern u32 collision_entity_count;
extern void *collision_entity_list;

/* Collision_TileRaycast @ 0x02066638 (1112 bytes)
 * Performs a raycast against the tile collision map.
 * Args: r0=x, r1=y, r2=coll_flags, r3=direction, sp+0=margin_ptr, sp+4=out_id
 *       sp+0x0c=height_ptr
 * Returns: collision depth (s32), 0=hit, positive=distance, negative=no hit */
s32 Collision_TileRaycast(s32 x, s32 y, u32 coll_flags, s32 direction,
                          void *margin_ptr, void *out_id, void *height_ptr) {
    s32 depth;
    s32 step_size;

    u8 margin_orig_a = 0;
    u8 margin_orig_b = 0;
    if (margin_ptr != NULL) {
        margin_orig_a = *(u8 *)margin_ptr;
    }
    if (out_id != NULL) {
        margin_orig_b = *(u8 *)out_id;
    }

    /* Determine if this is a vertical raycast (bit 5 of coll_flags) */
    if (coll_flags & 0x20) {
        switch (direction) {
            case 3:
                depth = g_camera_y - y;
                break;
            case 2:
                depth = y - g_camera_y;
                break;
            case 1:
                depth = x - g_camera_x;
                break;
            case 0:
            default:
                depth = g_camera_x - x;
                break;
        }

        if (depth < -30) {
            return depth;
        }
        if (depth > 31) {
            depth = 31;
        }
        return depth;
    }

    s32 margin_x = 0;
    if (margin_ptr != NULL) {
        margin_x = *(u8 *)margin_ptr;
    }

    u32 dir_flag = direction & 1;
    if (dir_flag) {
        step_size = -7;
    } else {
        step_size = 8;
    }

    if (direction & 2) {
        s32 tmp = x ^ y;
        y = y ^ tmp;
        x = tmp ^ y;

        s32 sub_offset = (y & 7) << 24;
        margin_x = sub_offset >> 24;

        extern s32 Collision_TileTest(s32 x, s32 y, u32 flags, s32 dir, void *mp, void *oid);
        s32 result = Collision_TileTest(x, y, coll_flags, direction, margin_ptr, out_id);

        if (result == 0) {
            if (margin_ptr != NULL) *(u8 *)margin_ptr = margin_orig_a;
            if (out_id != NULL) *(u8 *)out_id = margin_orig_b;

            s32 tile_offset = 8 - margin_x;
            if (step_size < 0) {
                tile_offset = -(margin_x + 1);
            }
            return tile_offset + 16;
        }

        if (result == 8) {
            s32 partial = (step_size > 0) ? (margin_x + 1) : -(margin_x + 1);
            return partial + 16;
        }

        if (result > 0) {
            s32 depth_adj = (step_size > 0) ? (margin_x + 1) : -(margin_x + 1);
            return result - depth_adj + 16;
        } else {
            s32 depth_adj = (step_size > 0) ? -(margin_x + 1) : (margin_x + 1);
            return result + depth_adj + 16;
        }
    }

    {
        s32 sub_x = x & 7;
        margin_x = (sub_x << 24) >> 24;

        extern s32 Collision_TileTest(s32 x, s32 y, u32 flags, s32 dir, void *mp, void *oid);
        s32 result = Collision_TileTest(x, y, coll_flags, direction, margin_ptr, out_id);

        if (result == 0) {
            if (margin_ptr != NULL) *(u8 *)margin_ptr = margin_orig_a;
            if (out_id != NULL) *(u8 *)out_id = margin_orig_b;

            s32 tile_offset = 8 - margin_x;
            if (step_size < 0) {
                tile_offset = -(margin_x + 1);
            }
            return tile_offset + 16;
        }

        if (result == 8) {
            s32 partial = (step_size > 0) ? (margin_x + 1) : -(margin_x + 1);
            return partial + 16;
        }

        if (result > 0) {
            s32 depth_adj = (step_size > 0) ? (margin_x + 1) : -(margin_x + 1);
            return result - depth_adj + 16;
        } else {
            s32 depth_adj = (step_size > 0) ? -(margin_x + 1) : (margin_x + 1);
            return result + depth_adj + 16;
        }
    }
}

/* Collision_SlopedSurfaceResponse @ 0x020670bc (1084 bytes)
 * Handles collision response on sloped surfaces.
 * Args: r0=entity, r1=x, r2=y, r3=direction, sp+0=coll_layer, sp+4=out_data
 * Returns: collision depth (s32) */
s32 Collision_SlopedSurfaceResponse(void *entity, s32 x, s32 y, s32 direction,
                                     u32 coll_layer, u32 out_data) {
    PhysicsPlayer *player = (PhysicsPlayer *)entity;
    s32 best_depth = 0x18;
    u32 entity_count = collision_entity_count;
    void **entity_list = (void **)&collision_entity_list;

    s32 slope_angle = player->slope_accel_x;
    s32 slope_base = player->slope_accel_y;

    if (slope_angle == 0 && slope_base == 0) {
        return best_depth;
    }

    s32 normal_x = -slope_angle;
    s32 normal_y = 0x100;

    s32 dot = (player->applied_vel_x * normal_x +
               player->applied_vel_y * normal_y) >> 8;

    for (u32 iter = 0; iter < entity_count; iter++) {
        void *ent = entity_list[iter];
        if (ent == NULL) continue;

        void *ent_data = *(void **)ent;
        if (ent_data == NULL || ent_data == entity) continue;

        u16 ent_w = *((u16 *)ent + 16);
        u16 ent_h = *((u16 *)ent + 17);
        if (ent_w == 0 || ent_h == 0) continue;

        s32 ent_x = *((s32 *)ent_data + 4) >> 8;
        s32 ent_y = *((s32 *)ent_data + 5) >> 8;

        s32 margin = 24;
        s32 dx = x - ent_x;
        s32 dy = y - ent_y;

        if (dx < -margin || dx > margin + (s32)ent_w * 8) continue;
        if (dy < -margin || dy > margin + (s32)ent_h * 8) continue;

        s32 penetration = (dx * normal_x + dy * normal_y) >> 8;

        if (penetration > 0 && penetration < best_depth) {
            best_depth = penetration;

            if (out_data != 0) {
                u32 *out = (u32 *)out_data;
                out[0] = (u32)(uintptr_t)ent;
                out[1] = (u32)penetration;
                out[2] = (u32)normal_x;
                out[3] = (u32)normal_y;
            }

            player->applied_vel_x -= (dot * normal_x) >> 8;
            player->applied_vel_y -= (dot * normal_y) >> 8;
        }
    }

    return best_depth;
}
