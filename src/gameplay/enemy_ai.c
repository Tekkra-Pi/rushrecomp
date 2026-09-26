/* Enemy AI functions - patrol and chase.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "entity.h"
#include "player.h"


/* Enemy_Patrol @ 0x02072000 (148 bytes)
 * Updates enemy patrol behavior.
 * Args: r0=enemy */
void Enemy_Patrol(EntityContainer *enemy) {
    s32 *data = (s32*)enemy->data;
    s32 patrol_speed = data[8];   /* +0x20: patrol speed */
    s32 patrol_range = data[9];   /* +0x24: patrol range */
    s32 patrol_origin = data[10]; /* +0x28: patrol origin X */
    
    /* Move in patrol direction */
    data[2] += patrol_speed * data[11];  /* +0x08: X += speed * direction */
    
    /* Check patrol bounds */
    s32 dist = data[2] - patrol_origin;
    if (dist > patrol_range || dist < -patrol_range) {
        /* Reverse direction */
        data[11] = -data[11];
    }
}

/* Enemy_ChasePlayer @ 0x02072094 (216 bytes)
 * Updates enemy chase behavior.
 * Args: r0=enemy */
void Enemy_ChasePlayer(EntityContainer *enemy) {
    s32 *data = (s32*)enemy->data;
    PhysicsPlayer *player = (PhysicsPlayer*)Entity_GetPlayer();
    if (!player) return;
    
    /* Calculate direction to player */
    s32 dx = player->pos_x - data[2];
    s32 dy = player->pos_y - data[3];
    
    /* Normalize and apply speed */
    s32 dist = Math_Sqrt(dx * dx + dy * dy);
    if (dist > 0) {
        s32 chase_speed = data[12];  /* +0x30: chase speed */
        data[2] += (dx * chase_speed) / dist;
        data[3] += (dy * chase_speed) / dist;
    }
    
    /* Update facing direction */
    data[11] = (dx > 0) ? 1 : -1;
}

/* Enemy_PatrolInit @ 0x02072170 (264 bytes)
 * Initializes enemy patrol.
 * Args: r0=enemy, r1=speed, r2=range */
void Enemy_PatrolInit(EntityContainer *enemy, s32 speed, s32 range) {
    s32 *data = (s32*)enemy->data;
    
    /* Set patrol parameters */
    data[8] = speed;    /* +0x20: patrol speed */
    data[9] = range;    /* +0x24: patrol range */
    data[10] = data[2]; /* +0x28: origin X = current X */
    data[11] = 1;       /* +0x2c: direction (1=right, -1=left) */
}

/* Enemy_ChaseInit @ 0x02072278
 * Initializes enemy chase.
 * Args: r0=enemy, r1=speed, r2=range */
void Enemy_ChaseInit(EntityContainer *enemy, s32 speed, s32 range) {
    s32 *data = (s32*)enemy->data;
    
    /* Set chase parameters */
    data[12] = speed;   /* +0x30: chase speed */
    data[13] = range;   /* +0x34: chase range */
}

/* Enemy_CanSeePlayer @ 0x020722e0
 * Checks if enemy can see player.
 * Args: r0=enemy
 * Returns: 1 if can see, 0 otherwise */
s32 Enemy_CanSeePlayer(EntityContainer *enemy) {
    s32 *data = (s32*)enemy->data;
    PhysicsPlayer *player = (PhysicsPlayer*)Entity_GetPlayer();
    if (!player) return 0;
    
    /* Check distance */
    s32 dx = player->pos_x - data[2];
    s32 dy = player->pos_y - data[3];
    s32 dist_sq = (dx * dx + dy * dy) >> 12;
    
    s32 sight_range = data[14];  /* +0x38: sight range */
    
    /* Check if within sight range */
    if (dist_sq < sight_range * sight_range) {
        return 1;
    }
    
    return 0;
}

/* Enemy_FacePlayer @ 0x02072360
 * Makes enemy face player.
 * Args: r0=enemy */
void Enemy_FacePlayer(EntityContainer *enemy) {
    s32 *data = (s32*)enemy->data;
    PhysicsPlayer *player = (PhysicsPlayer*)Entity_GetPlayer();
    if (!player) return;
    
    /* Update facing direction */
    data[11] = (player->pos_x > data[2]) ? 1 : -1;
}
