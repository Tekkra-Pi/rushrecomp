/* Collision system functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 80-116
 * collision_dispatcher @ 0x02066010 (160 bytes)
 * entity_aabb_sweep @ 0x02067670 (936 bytes) */

#include "nds_types.h"
#include "player.h"

/* Forward declarations */
extern s32 Collision_TileRaycast(s32 x, s32 y, u32 flags, s32 dir, void *mp, void *oid, void *hp);
extern s32 Collision_SlopedSurfaceResponse(void *entity, s32 x, s32 y, s32 dir, u32 a, u32 b);
extern void Math_SinCos(s32 angle, s32 *sin_out, s32 *cos_out);
s32 Entity_AABBSweep(void *entity, s32 x, s32 y, s32 dir, u32 coll_layer, s32 *out_count, u32 *out_id);

/* Global collision entity data (from symbols) */
extern u32 collision_entity_count;
extern void *collision_entity_list;

/* Collision_Dispatcher @ 0x02066010 (160 bytes)
 * Dispatches collision resolution based on player state flags.
 * Args: r0=player, r1=x, r2=y, r3=dir, sp+0=coll_layer, sp+4=margin_flag,
 *       sp+8=out_count, sp+0x0c=out_id
 * Returns: collision depth (s32) */
s32 Collision_Dispatcher(PhysicsPlayer *player, s32 x, s32 y, s32 dir,
                         u32 coll_layer, u8 margin_flag, s32 *out_count, u32 *out_id) {
    s32 depth = 0x20;

    /* Check state_flags bit 12 (0x1000) - skip tile raycast if set */
    if (!(player->state_flags & 0x1000)) {
        s32 ray_result = Collision_TileRaycast(x, y, coll_layer, dir, margin_flag, out_count, out_id);
        depth = ray_result;
    }

    /* Check state_flags bit 9 (0x200) - skip entity sweep if set */
    if (!(player->state_flags & 0x200)) {
        s32 sweep_result = Entity_AABBSweep(player, x, y, dir, coll_layer, out_count, out_id);
        if (sweep_result < depth) {
            depth = sweep_result;
        }
    }

    return depth;
}

/* Entity_AABBSweep @ 0x02067670 (936 bytes)
 * Sweeps all collision entities against an AABB from a given position/direction.
 * Args: r0=player, r1=x, r2=y, r3=dir, sp+0=coll_layer, sp+4=margin_flag,
 *       sp+8=out_count, sp+0x0c=out_id
 * Returns: closest collision depth (s32), or 24 if none found */
s32 Entity_AABBSweep(void *entity, s32 x, s32 y, s32 dir,
                     u32 coll_layer, s32 *out_count, u32 *out_id) {
    s32 best_depth = 24;
    u32 entity_count = collision_entity_count;
    void **entity_list = (void **)&collision_entity_list;
    PhysicsPlayer *player = (PhysicsPlayer *)entity;

    u8 player_dir = ((u8 *)player)[5];
    u32 state_flags = player->state_flags;
    s32 should_check = 0;

    s32 dir_class = ((player_dir + 32) & 0xC0) >> 6;

    switch (dir_class) {
        case 0:
            if (dir == 2) {
                if ((state_flags & 0x10) == 0 || player->applied_vel_x >= 0) {
                    should_check = 1;
                }
            }
            break;
        case 1:
            if (dir == 1) {
                if ((state_flags & 0x10) == 0 || player->applied_vel_y < 0) {
                    should_check = 1;
                }
            }
            break;
        case 2:
            if (dir == 3) {
                if ((state_flags & 0x10) == 0 || player->applied_vel_y >= 0) {
                    should_check = 1;
                }
            }
            break;
        case 3:
            if (dir == 0) {
                if ((state_flags & 0x10) == 0 || player->applied_vel_y > 0) {
                    should_check = 1;
                }
            }
            break;
    }

    if (entity_count == 0) {
        return best_depth;
    }

    for (u32 iter = 0; iter < entity_count; iter++) {
        void *ent = entity_list[iter];
        if (ent == NULL) continue;

        void *ent_data = *(void **)ent;
        if (ent_data == NULL || ent_data == entity) continue;

        u16 ent_w = *((u16 *)ent + 16);
        u16 ent_h = *((u16 *)ent + 17);
        if (ent_w == 0 || ent_h == 0) continue;

        s32 ent_offset_x = 0;
        s32 ent_offset_y = 0;

        u8 active = *((u8 *)ent_data + 1);
        if (active != 0) {
            s32 evx = *((s32 *)ent_data + 4);
            s32 evy = *((s32 *)ent_data + 5);
            ent_offset_x = (x << 8) - evx;
            ent_offset_y = (y << 8) - evy;
        }

        s32 angle_byte = -((s32)(s8)(*((u8 *)ent_data + 1)) << 8);
        s32 sin_val, cos_val;
        Math_SinCos(angle_byte, &sin_val, &cos_val);

        s32 rotated_x = (ent_offset_x * cos_val - ent_offset_y * sin_val) >> 8;
        s32 rotated_y = (ent_offset_x * sin_val + ent_offset_y * cos_val) >> 8;

        {
            s32 ex = *((s32 *)ent_data + 4);
            s32 ey = *((s32 *)ent_data + 5);
            x = (ex + rotated_x) >> 8;
            y = (ey + rotated_y) >> 8;
        }

        {
            s32 ent_x_fixed = *((s32 *)(*(void **)ent) + 4) >> 8;
            s32 margin = 24;
            if (x < ent_x_fixed - margin) continue;
            if (x > ent_x_fixed + (s32)ent_w * 8 + margin) continue;

            s32 ent_y_fixed = *((s32 *)(*(void **)ent) + 5) >> 8;
            if (y < ent_y_fixed - margin) continue;
            if (y > ent_y_fixed + (s32)ent_h * 8 + margin) continue;
        }

        s32 result = Collision_SlopedSurfaceResponse(ent, x, y, dir, (u32)out_count, 0);

        if (result < best_depth) {
            best_depth = result;

            if (result <= 0) {
                player->partner = *(void **)ent;
            }

            if (should_check) {
                player->partner_ref = *(void **)ent;

                if (out_id != NULL) {
                    u8 dir_byte = player_dir;
                    if (dir_byte != 0) {
                        u8 stored = *out_id;
                        *out_id = stored + dir_byte;
                    }
                }
            } else {
                if (out_id != NULL) {
                    u8 dir_byte = player_dir;
                    if (dir_byte != 0) {
                        *out_id = dir_byte;
                    }
                }
            }
        }
    }

    return best_depth;
}
