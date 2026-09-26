/* Collision resolution functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "player.h"

/* CollisionResolve_Ground @ 0x0206a400 (168 bytes)
 * Resolves ground collision.
 * Args: r0=player, r1=ground_data */
void CollisionResolve_Ground(PhysicsPlayer *player, s32 *ground_data) {
    s32 ground_y = ground_data[0];
    s32 slope_angle = ground_data[1];
    
    /* Snap player to ground */
    player->pos_y = ground_y;
    player->applied_vel_y = 0;
    
    /* Set grounded flag */
    player->state_flags |= 0x01;
    
    /* Apply slope velocity */
    if (slope_angle != 0) {
        player->applied_vel_x += (slope_angle * player->applied_vel_y) >> 8;
    }
}

/* CollisionResolve_Wall @ 0x0206a4a8 (232 bytes)
 * Resolves wall collision.
 * Args: r0=player, r1=wall_data */
void CollisionResolve_Wall(PhysicsPlayer *player, s32 *wall_data) {
    s32 wall_x = wall_data[0];
    s32 wall_normal = wall_data[1];
    
    /* Push player away from wall */
    if (wall_normal > 0) {
        player->pos_x = wall_x - 16;
    } else {
        player->pos_x = wall_x + 16;
    }
    
    /* Stop horizontal velocity into wall */
    if ((wall_normal > 0 && player->applied_vel_x > 0) ||
        (wall_normal < 0 && player->applied_vel_x < 0)) {
        player->applied_vel_x = 0;
    }
    
    /* Set wall contact flag */
    player->state_flags |= 0x04;
    player->wall_direction = wall_normal;
}

/* CollisionResolve_Ceiling @ 0x0206a590 (296 bytes)
 * Resolves ceiling collision.
 * Args: r0=player, r1=ceiling_data */
void CollisionResolve_Ceiling(PhysicsPlayer *player, s32 *ceiling_data) {
    s32 ceiling_y = ceiling_data[0];
    
    /* Push player below ceiling */
    player->pos_y = ceiling_y + 32;
    
    /* Bounce off ceiling */
    if (player->applied_vel_y < 0) {
        player->applied_vel_y = -player->applied_vel_y / 2;
    }
}

/* CollisionResolve_Slope @ 0x0206a6c8
 * Resolves slope collision.
 * Args: r0=player, r1=slope_data */
void CollisionResolve_Slope(PhysicsPlayer *player, s32 *slope_data) {
    s32 slope_height = slope_data[0];
    s32 slope_angle = slope_data[1];
    
    /* Snap to slope surface */
    player->pos_y = slope_height;
    player->applied_vel_y = 0;
    
    /* Set grounded flag */
    player->state_flags |= 0x01;
    
    /* Apply slope acceleration */
    s32 slope_accel = (slope_angle * 0x100) >> 8;
    player->applied_vel_x += slope_accel;
}

/* CollisionResolve_Push @ 0x0206a780
 * Resolves push collision (pushable objects).
 * Args: r0=player, r1=push_data */
void CollisionResolve_Push(PhysicsPlayer *player, s32 *push_data) {
    s32 push_x = push_data[0];
    s32 push_force = push_data[1];
    
    /* Push object */
    if (player->applied_vel_x > 0) {
        push_x += push_force;
    } else {
        push_x -= push_force;
    }
    
    push_data[0] = push_x;
}
