/* Collision response extended functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "player.h"

/* CollisionResponse_Bounce
 * Bounces an entity off a surface with energy loss.
 * Args: r0=entity, r1=normal_x, r2=normal_y, r3=bounce_factor */
void CollisionResponse_Bounce(PhysicsPlayer *entity, s32 normal_x, s32 normal_y, s32 bounce_factor) {
    /* Project velocity onto surface normal */
    s32 dot = (entity->applied_vel_x * normal_x +
               entity->applied_vel_y * normal_y) >> 8;

    /* Remove normal component and apply bounce */
    entity->applied_vel_x = entity->applied_vel_x - (dot * normal_x >> 8);
    entity->applied_vel_y = entity->applied_vel_y - (dot * normal_y >> 8);

    /* Apply bounce factor */
    entity->applied_vel_x = (entity->applied_vel_x * bounce_factor) >> 8;
    entity->applied_vel_y = (entity->applied_vel_y * bounce_factor) >> 8;

    /* Add reflected normal component */
    entity->applied_vel_x += (dot * normal_x * bounce_factor) >> 8;
    entity->applied_vel_y += (dot * normal_y * bounce_factor) >> 8;
}

/* CollisionResponse_Slide
 * Slides an entity along a surface (removes normal component).
 * Args: r0=entity, r1=normal_x, r2=normal_y */
void CollisionResponse_Slide(PhysicsPlayer *entity, s32 normal_x, s32 normal_y) {
    /* Project velocity onto surface normal */
    s32 dot = (entity->applied_vel_x * normal_x +
               entity->applied_vel_y * normal_y) >> 8;

    /* Remove normal component (slide along surface) */
    entity->applied_vel_x -= (dot * normal_x) >> 8;
    entity->applied_vel_y -= (dot * normal_y) >> 8;
}

/* CollisionResponse_Stick
 * Sticks an entity to a surface (zeroes velocity into surface).
 * Args: r0=entity, r1=normal_x, r2=normal_y */
void CollisionResponse_Stick(PhysicsPlayer *entity, s32 normal_x, s32 normal_y) {
    /* Project velocity onto surface normal */
    s32 dot = (entity->applied_vel_x * normal_x +
               entity->applied_vel_y * normal_y) >> 8;

    /* Zero velocity into surface */
    entity->applied_vel_x -= (dot * normal_x) >> 8;
    entity->applied_vel_y -= (dot * normal_y) >> 8;

    /* Zero any remaining perpendicular component into surface */
    if (dot < 0) {
        entity->applied_vel_x = 0;
        entity->applied_vel_y = 0;
    }
}

/* CollisionResponse_Gravity
 * Applies gravity response to an entity.
 * Args: r0=entity, r1=gravity, r2=terminal_vel */
void CollisionResponse_Gravity(PhysicsPlayer *entity, s32 gravity, s32 terminal_vel) {
    /* Apply gravity to vertical velocity */
    entity->applied_vel_y += gravity;

    /* Clamp to terminal velocity */
    if (entity->applied_vel_y > terminal_vel) {
        entity->applied_vel_y = terminal_vel;
    }
}

/* CollisionResponse_Reset
 * Resets collision response state for an entity.
 * Args: r0=entity */
void CollisionResponse_Reset(PhysicsPlayer *entity) {
    /* Clear applied velocities */
    entity->applied_vel_x = 0;
    entity->applied_vel_y = 0;

    /* Clear state flags related to collision */
    entity->state_flags &= ~(PLAYER_STATE_GROUNDED | PLAYER_STATE_IMPACT);
}
