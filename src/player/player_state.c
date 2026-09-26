/* Player state management functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "player.h"

/* Player_GetState @ 0x0206fd70 (408 bytes)
 * Returns player state based on flags and position.
 * Args: r0=player
 * Returns: state_id */
u32 Player_GetState(PhysicsPlayer *player) {
    u32 state = 0;
    
    /* Check grounded flag */
    if (player->state_flags & 0x01) {
        /* Grounded */
        if (player->applied_vel_x != 0) {
            state = 0x01;  /* Walking/running */
        } else {
            state = 0x00;  /* Idle */
        }
    } else {
        /* Airborne */
        if (player->applied_vel_y < 0) {
            state = 0x02;  /* Jumping */
        } else {
            state = 0x03;  /* Falling */
        }
    }
    
    /* Check special states */
    if (player->control_flags & 0x04) {
        state = 0x04;  /* Dead */
    }
    if (player->control_flags & 0x40) {
        state = 0x05;  /* Invulnerable */
    }
    
    return state;
}

/* Player_SetState @ 0x0206ff08 (288 bytes)
 * Sets player state and updates flags accordingly.
 * Args: r0=player, r1=new_state */
void Player_SetState(PhysicsPlayer *player, u32 new_state) {
    u32 old_state = Player_GetState(player);
    
    /* Clear old state flags */
    player->state_flags &= ~0x0f;
    
    /* Set new state flags */
    switch (new_state) {
        case 0x00:  /* Idle */
            player->state_flags |= 0x01;  /* GROUNDED */
            break;
            
        case 0x01:  /* Walking */
            player->state_flags |= 0x01;  /* GROUNDED */
            break;
            
        case 0x02:  /* Jumping */
            player->state_flags &= ~0x01;  /* Clear GROUNDED */
            player->applied_vel_y = -0x400;  /* Jump velocity */
            break;
            
        case 0x03:  /* Falling */
            player->state_flags &= ~0x01;  /* Clear GROUNDED */
            break;
            
        case 0x04:  /* Dead */
            player->control_flags |= 0x04;
            break;
            
        case 0x05:  /* Invulnerable */
            player->control_flags |= 0x40;
            player->invuln_timer = 120;  /* 2 seconds */
            break;
            
        default:
            break;
    }
    
    /* Call state change callback */
    if (player->state_change_fn) {
        player->state_change_fn(player, old_state, new_state);
    }
}

/* Player_IsGrounded @ 0x02070028
 * Returns whether player is on ground.
 * Args: r0=player
 * Returns: 1 if grounded, 0 otherwise */
s32 Player_IsGrounded(PhysicsPlayer *player) {
    return (player->state_flags & 0x01) ? 1 : 0;
}

/* Player_IsAlive @ 0x02070048
 * Returns whether player is alive.
 * Args: r0=player
 * Returns: 1 if alive, 0 otherwise */
s32 Player_IsAlive(PhysicsPlayer *player) {
    return (player->control_flags & 0x04) ? 0 : 1;
}
