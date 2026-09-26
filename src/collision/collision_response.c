/* Collision response and platform functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "player.h"


/* Collision_Response @ 0x02071758 (376 bytes)
 * Handles collision response for player.
 * Args: r0=player, r1=collision_data */
void Collision_Response(PhysicsPlayer *player, s32 *collision_data) {
    s32 collision_type = collision_data[0];
    s32 normal_x = collision_data[1];
    s32 normal_y = collision_data[2];
    s32 penetration = collision_data[3];
    
    /* Apply response based on collision type */
    switch (collision_type) {
        case 0x01:  /* Ground */
            /* Snap to ground */
            player->pos_y += penetration;
            player->applied_vel_y = 0;
            player->state_flags |= 0x01;  /* GROUNDED */
            break;
            
        case 0x02:  /* Wall */
            /* Stop horizontal movement */
            player->pos_x += penetration;
            player->applied_vel_x = 0;
            break;
            
        case 0x03:  /* Ceiling */
            /* Bounce off ceiling */
            player->pos_y += penetration;
            player->applied_vel_y = -player->applied_vel_y / 2;
            break;
            
        default:
            break;
    }
}

/* Collision_ResponseSlope @ 0x020718d0 (312 bytes)
 * Handles slope collision response.
 * Args: r0=player, r1=slope_data */
void Collision_ResponseSlope(PhysicsPlayer *player, s32 *slope_data) {
    s32 slope_angle = slope_data[0];
    s32 slope_height = slope_data[1];
    
    /* Calculate surface normal from angle */
    s32 normal_x = -slope_angle;
    s32 normal_y = 0x100;  /* Up vector */
    
    /* Project velocity onto surface normal */
    s32 dot = (player->applied_vel_x * normal_x + 
               player->applied_vel_y * normal_y) >> 8;
    
    /* Remove component into surface */
    player->applied_vel_x -= (dot * normal_x) >> 8;
    player->applied_vel_y -= (dot * normal_y) >> 8;
    
    /* Snap to surface */
    player->pos_y = slope_height;
    player->state_flags |= 0x01;  /* GROUNDED */
}

/* Collision_ResponseWall @ 0x02071a08 (264 bytes)
 * Handles wall collision response with sliding.
 * Args: r0=player, r1=wall_data */
void Collision_ResponseWall(PhysicsPlayer *player, s32 *wall_data) {
    s32 wall_normal = wall_data[0];
    s32 penetration = wall_data[1];
    
    /* Push player out of wall */
    if (wall_normal > 0) {
        player->pos_x += penetration;
    } else {
        player->pos_x -= penetration;
    }
    
    /* Stop horizontal velocity into wall */
    if ((wall_normal > 0 && player->applied_vel_x > 0) ||
        (wall_normal < 0 && player->applied_vel_x < 0)) {
        player->applied_vel_x = 0;
    }
}

/* Platform_Update @ 0x02072794 (228 bytes)
 * Updates platform object position.
 * Args: r0=platform */
void Platform_Update(void *platform) {
    s32 *data = (s32*)platform;
    s32 pos_x = data[2];   /* +0x08 */
    s32 pos_y = data[3];   /* +0x0c */
    s32 vel_x = data[4];   /* +0x10 */
    s32 vel_y = data[5];   /* +0x14 */
    u32 state = data[1];   /* +0x04 */
    
    /* Update position based on velocity */
    data[2] = pos_x + vel_x;
    data[3] = pos_y + vel_y;
    
    /* Check for endpoint */
    if (state & 0x01) {
        /* Moving platform - check bounds */
        /* ... */
    }
}

/* Spring_Apply @ 0x02072878 (196 bytes)
 * Applies spring force to player.
 * Args: r0=player, r1=spring_data */
void Spring_Apply(PhysicsPlayer *player, s32 *spring_data) {
    s32 spring_force = spring_data[0];
    s32 spring_angle = spring_data[1];
    
    /* Apply spring force */
    s32 vel_x = (spring_force * Math_Sin(spring_angle)) >> 8;
    s32 vel_y = (spring_force * Math_Cos(spring_angle)) >> 8;
    
    /* Add to player velocity */
    player->applied_vel_x += vel_x;
    player->applied_vel_y += vel_y;
    
    /* Clear grounded flag */
    player->state_flags &= ~0x01;
}

/* Spring_Init @ 0x02072940 (140 bytes)
 * Initializes spring object.
 * Args: r0=spring, r1=force, r2=angle */
void Spring_Init(void *spring, s32 force, s32 angle) {
    s32 *data = (s32*)spring;
    
    /* Set spring parameters */
    data[0] = force;    /* +0x00: force */
    data[1] = angle;    /* +0x04: angle */
    data[2] = 0;        /* +0x08: state */
    data[3] = 0;        /* +0x0c: timer */
}
