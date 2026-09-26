/* Player system functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 324-450 */

#include "nds_types.h"
#include "player.h"

/* Helper: absolute value (no stdlib) */
static inline s32 absv(s32 x) { return x < 0 ? -x : x; }

/* Forward declarations for external functions */
extern void Player_PhysicsUpdate(PhysicsPlayer *player);
extern s32 Collision_TestHitbox(s32 pos_x, s32 pos_y, s32 hitbox_x, s32 hitbox_y,
                                s32 saved_vx, s32 saved_vy, s32 saved_vz);
extern void Player_PhysicsStep(PhysicsPlayer *player);
extern void ActionState_Cleanup(PhysicsPlayer *player);
extern void HitboxLink_Update(PhysicsPlayer *player, void *group_a, void *group_b);
extern s32 Player_CollisionIterate(PhysicsPlayer *player);
extern void Player_DeathSequence(void);

/* Global pointer references (from literal pools) */
extern u32 *g_player_ptr;
extern u32 g_system_flags;
extern const s8 g_drift_table[];
extern void ActionTrigger_Setup(void *player, void *callback, u32 idx, u32 param);

/* Forward declarations */
void Movement_Iterate(PhysicsPlayer *player);

/* ======================================================================== */
/* Player_BusyGate @ 0x0206c798 (36 bytes)                                  */
/* Checks if player update should be gated (skipped).                       */
/* When game is paused and player is not pausable, returns 1 (busy).        */
/* Args: r0=control_flags                                                   */
/* Returns: 1 if gated (skip update), 0 if allowed                          */
/* ======================================================================== */
s32 Player_BusyGate(u32 control_flags) {
    /* Check system pause flag (bit 0 of g_system_flags) */
    if (g_system_flags & 1) {
        /* Game is paused - check if player is pausable (0x20) */
        if (!(control_flags & PLAYER_CTRL_PAUSABLE)) {
            return 1;  /* Not pausable, gate the update */
        }
    }
    return 0;  /* Not gated */
}

/* ======================================================================== */
/* Player_Destroy @ 0x0206bef4 (64 bytes)                                   */
/* Entity container destroy callback for the player.                        */
/* Cleans up state halfword, frees action data, then tails to entity free.  */
/* Args: r0=EntityContainer* (offset +0x10 = PhysicsPlayer*)                */
/* ======================================================================== */
void Player_Destroy(void *container) {
    PhysicsPlayer *player = *(PhysicsPlayer **)((u8 *)container + 16);

    /* +0x54: control_flags - check PLAYER_STATE_V (0x800) */
    if (player->control_flags & 0x800) {
        /* +0xd8: state_halfword - external cleanup */
    }

    /* +0xd8: state_halfword low byte - external processing */
    {
        u16 st = player->state_halfword;  /* +0xd8 */
        /* extern fn(st & 0xFF) */
        (void)st;
    }

    /* +0x8c: action_data - free if allocated */
    if (player->action_data) {
        /* extern fn: action_data_free(player->action_data) */
        player->action_data = NULL;
    }

    /* Note: function tail-calls into entity removal code at 0x0206bfa8 */
}

/* ======================================================================== */
/* Player_Update @ 0x0206c3d0 (812 bytes)                                   */
/* Main per-frame player update. Called from entity walker.                 */
/* Args: r0=EntityContainer* (->data at +0x10 = PhysicsPlayer*)             */
/* ======================================================================== */
void Player_Update(void *container) {
    PhysicsPlayer *player;
    u32 flags;
    u16 saved_status;

    /* +0x10: data ptr = PhysicsPlayer* */
    player = *(PhysicsPlayer **)((u8 *)container + 16);

    /* +0x54: control_flags - early exit if PLAYER_CTRL_BUSY (0x04) */
    flags = player->control_flags;
    if (flags & PLAYER_CTRL_BUSY) {
        Player_DeathSequence();
        return;
    }

    /* If PLAYER_CTRL_P (0x08) set, force BUSY */
    if (flags & PLAYER_CTRL_P) {
        player->control_flags = flags | PLAYER_CTRL_BUSY;
    }

    /* If PLAYER_CTRL_Q (0x10) set, skip collision check */
    if (!(player->control_flags & PLAYER_CTRL_Q)) {
        s32 hit = Collision_TestHitbox(
            player->pos_x, player->pos_y,
            player->hitbox_offset_x, player->hitbox_offset_y,
            player->saved_vel_x, player->saved_vel_y, player->saved_vel_z);
        if (hit) {
            player->control_flags |= PLAYER_CTRL_BUSY;
        }
    }

    /* +0x88: partner_ref - check if partner is busy */
    if (player->partner_ref) {
        u32 partner_flags = *(u32 *)((u8 *)player->partner_ref + 0x54);
        if (partner_flags & PLAYER_CTRL_BUSY) {
            if (!(player->control_flags & 0x200)) {
                player->control_flags |= PLAYER_CTRL_BUSY;
            }
            player->partner_ref = NULL;
        }
    }

    /* Main physics update */
    Player_PhysicsUpdate(player);

    /* Gate check #1: if not gated OR bit 6 clear, call action_fn_2 */
    if (!Player_BusyGate(player->control_flags) ||
        !(player->control_flags & 0x40)) {
        if (player->action_fn_2) {  /* +0x6c */
            player->action_fn_2(player);
        }
    }

    /* Gate check #2: skip timer if gated */
    if (Player_BusyGate(player->control_flags)) {
        goto post_timer;
    }

    /* +0x62: action_timer - decrement low nibble (phase timer) */
    if (player->action_timer & 0x0F) {
        player->action_timer--;
    }
    /* Decrement high nibble by 0x10 (drift index) */
    if (player->action_timer & 0xF0) {
        player->action_timer -= 0x10;
    }

    /* Gate check #3: skip action_fn if gated */
    if (Player_BusyGate(player->control_flags)) {
        goto post_timer;
    }

    /* If phase timer hit 0, call action_fn (unless CTRL_R set) */
    if (!(player->action_timer & 0x0F)) {
        if (!(player->control_flags & PLAYER_CTRL_R)) {
            if (player->action_fn) {  /* +0x68 */
                player->action_fn(player);
            }
        }
    }

    /* +0x58: state_flags - run physics unless frozen (0x2000) */
    if (!(player->state_flags & PLAYER_STATE_FROZEN)) {
        Player_PhysicsStep(player);
    }

    /* System flag bit 3: movement iteration (if not spawning) */
    if (g_system_flags & 8) {
        if (!(player->state_flags & PLAYER_STATE_SPAWNING)) {
            player->object_binding = NULL;  /* +0x80 */
            player->partner = NULL;         /* +0x84 */
            Movement_Iterate(player);
        }
    }

post_timer:
    /* Save status, then set bit 4 if timer active or gated */
    saved_status = player->status;  /* +0x5c */
    if ((player->action_timer & 0x0F) || Player_BusyGate(player->control_flags)) {
        player->status |= 0x10;
    }

    /* Velocity drift table application */
    if (player->action_timer & 0xF0) {
        s32 idx = (s32)((player->action_timer & 0xF0) >> 5);
        s8 *slope_x = (s8 *)((u8 *)player + 6);
        s8 *slope_y = (s8 *)((u8 *)player + 7);
        *slope_x += g_drift_table[idx];
        *slope_y += g_drift_table[idx + 1];
    }

    /* Action state processing (only when not busy) */
    if (!(player->control_flags & PLAYER_CTRL_BUSY)) {
        /* +0x70: action_fn_3 */
        if (player->action_fn_3) {
            player->action_fn_3(player);
        }

        /* If no action_fn_3 or status bit 6 set */
        if (!player->action_fn_3 || (player->status & 0x40)) {
            /* Check action states (+0x124, +0x128, +0x12c) */
            if (player->action_state_a || player->action_state_b ||
                player->action_state_c) {
                ActionState_Cleanup(player);
            } else if (player->hitbox_link_fn) {  /* +0xa4 */
                HitboxLink_Update(player, &player->hitbox_group_a,
                                  &player->hitbox_group_b);
            }
        }
    }

    /* Restore status if timer running or gated */
    if ((player->action_timer & 0x0F) || Player_BusyGate(player->control_flags)) {
        player->status = saved_status;
    }

    /* Final gate check */
    if (Player_BusyGate(player->control_flags)) {
        return;
    }

    /* If timer expired and system flag bit 4, call misc_fn_1 */
    if (!(player->action_timer & 0x0F)) {
        if (g_system_flags & 16) {
            if (player->misc_fn_1) {  /* +0x78 */
                player->misc_fn_1(player);
            }
        }
    }

    /* Clear per-frame scratch */
    player->raw_input_x = 0;  /* +0x36 */
    player->raw_input_y = 0;  /* +0x38 */
    player->raw_input_z = 0;  /* +0x3a */
    *(s8 *)((u8 *)player + 6) = 0;  /* +0x06 */
    *(s8 *)((u8 *)player + 7) = 0;  /* +0x07 */

    /* +0x7c: misc_fn_2 */
    if (player->misc_fn_2) {
        player->misc_fn_2(player);
    }
}

/* ======================================================================== */
/* Movement_Iterate @ 0x0206bd5c (408 bytes)                                */
/* Iteratively moves player toward target with collision checking.          */
/* Steps position in threshold-sized increments, resolving collisions.      */
/* Args: r0=PhysicsPlayer*                                                  */
/* ======================================================================== */
void Movement_Iterate(PhysicsPlayer *player) {
    s32 saved_target_x, saved_target_y;
    s32 orig_pos_x, orig_pos_y;
    u16 saved_prev_angle;
    u32 state_flags;
    u32 accum_flags;
    u32 threshold;

    saved_target_x = player->target_x;  /* +0x1c */
    saved_target_y = player->target_y;  /* +0x20 */
    orig_pos_x = player->pos_x;         /* +0x10 */
    orig_pos_y = player->pos_y;         /* +0x14 */
    saved_prev_angle = player->prev_angle;  /* +0x5e */

    /* +0x60: store prev_angle, then clear +0x5e */
    player->prev_angle_stored = saved_prev_angle;
    player->prev_angle = 0;

    state_flags = player->state_flags;  /* +0x58 */
    accum_flags = 0;
    threshold = 0x800;

    /* Skip iteration if spawning */
    if (state_flags & PLAYER_STATE_SPAWNING) {
        goto finish;
    }

    /* Skip if direction flag 0x200000 not set */
    if (!(state_flags & PLAYER_STATE_DIR_200000)) {
        goto finish;
    }

    /* Wider threshold when airborne */
    if (!(state_flags & PLAYER_STATE_GROUNDED)) {
        threshold = 0x4000;
    }

step_loop:
    /* Check if position exceeds threshold from target on X or Y */
    if (absv(player->pos_x - player->target_x) > threshold ||
        absv(player->pos_y - player->target_y) > threshold) {
        /* Snap position to target */
        player->pos_x = player->target_x;
        player->pos_y = player->target_y;

        /* X axis: check overshoot from original pos */
        if (absv(player->pos_x - orig_pos_x) > threshold) {
            player->target_x = player->pos_x;
            if (orig_pos_x > player->target_x) {
                player->pos_x = player->target_x + threshold;
            } else {
                player->pos_x = player->target_x - threshold;
            }
        } else {
            player->pos_x = orig_pos_x;
        }

        /* Y axis: check overshoot from original pos */
        if (absv(player->pos_y - orig_pos_y) > threshold) {
            player->target_y = player->pos_y;
            if (orig_pos_y > player->target_y) {
                player->pos_y = player->target_y + threshold;
            } else {
                player->pos_y = player->target_y - threshold;
            }
        } else {
            player->pos_y = orig_pos_y;
        }
    }

    /* Check if position changed */
    if (player->pos_x == orig_pos_x && player->pos_y == orig_pos_y) {
        goto finish;
    }

    /* Collision check */
    Player_CollisionIterate(player);

    /* Track which axes changed, accumulate collision/state flags */
    if (player->pos_x != orig_pos_x) {
        orig_pos_x = player->pos_x;
    }
    accum_flags |= player->prev_angle;  /* +0x5e */
    accum_flags |= (player->state_flags & 0x0F);  /* +0x58 low nibble */

    if (player->pos_y != orig_pos_y) {
        orig_pos_y = player->pos_y;
    }

    goto step_loop;

finish:
    /* Final collision resolve */
    Player_CollisionIterate(player);

    /* Merge accumulated flags */
    player->prev_angle |= (u16)(accum_flags & 0xFFFF);
    player->state_flags |= accum_flags;

    /* Restore original target values */
    player->target_x = saved_target_x;
    player->target_y = saved_target_y;
}

/* ======================================================================== */
/* Action_Dispatch @ ~0x0206bf34 (body in 0x0206c000-0x0206c278)            */
/* Dispatches action state objects: processes 5-slot array for state A,     */
/* linked list for state B, and single object for state C.                 */
/* Args: r0=PhysicsPlayer*                                                  */
/* ======================================================================== */
void Action_Dispatch(PhysicsPlayer *player) {
    void *state_a;      /* +0x124: action_state_a */
    void *state_b;      /* +0x128: action_state_b */
    void *state_c;      /* +0x12c: action_state_c */
    u32 i;

    state_a = player->action_state_a;

    /* --- Action State A: 5-slot array processing --- */
    if (state_a) {
        u32 *base = (u32 *)state_a;
        u32 lock_flags = base[0x168 / 4];  /* +0x168: lock/skip bitmask */

        /* Slot 0: check +0x138 field */
        {
            void *slot0 = (void *)base[0x138 / 4];
            if (slot0) {
                u16 ref = *(u16 *)((u8 *)slot0 + 4);
                if (ref == 1) {
                    /* ref==1: cleanup resource */
                }
                /* Free slot and clear */
                base[0x138 / 4] = 0;
            } else {
                /* Check +0x13c linked data */
                void *linked = (void *)base[0x13c / 4];
                if (linked) {
                    if (!(lock_flags & (2u << 0))) {
                        /* Process linked entry */
                    }
                }
            }
        }

        /* Slots 1-4: iterate with index */
        for (i = 1; i < 5; i++) {
            u32 slot_offset = 0x13c + i * 4;
            u32 data_offset = 0x154 + i * 4;

            void *slot_data = (void *)base[data_offset / 4];
            if (slot_data) {
                u16 ref = *(u16 *)((u8 *)slot_data + 4);
                if (ref == 1) {
                    /* ref==1: cleanup */
                }
                /* Free and clear */
                base[slot_offset / 4] = 0;
            } else {
                void *linked = (void *)base[slot_offset / 4];
                if (linked) {
                    if (!(lock_flags & (2u << i))) {
                        /* Process linked entry */
                    }
                }
            }
        }
    }

    /* --- Action State B: linked list traversal --- */
    state_b = player->action_state_b;
    if (state_b) {
        void *entry = state_b;
        while (entry) {
            void *link_fn = *(void **)((u8 *)entry + 164);  /* +0xa4 */

            if (link_fn) {
                u16 type = *(u16 *)((u8 *)link_fn + 4);
                if (type == 1) {
                    /* Cleanup */
                }
                *(void **)((u8 *)entry + 160) = NULL;  /* +0xa0 = NULL */
            } else {
                void *data = *(void **)((u8 *)entry + 160);  /* +0xa0 */
                if (data) {
                    u32 flags = *(u32 *)((u8 *)entry + 168);  /* +0xa8 */
                    if (!(flags & 1)) {
                        /* Process entry */
                    }
                }
            }

            entry = *(void **)((u8 *)entry + 172);  /* +0xac: next */
        }
    }

    /* --- Action State C: single object --- */
    state_c = player->action_state_c;
    if (!state_c) {
        return;
    }
    if (*(u32 *)((u8 *)state_c + 256) == 0) {  /* +0x100 */
        return;
    }
    /* Continue processing action_state_c... */
}

/* ======================================================================== */
/* Action_DecrementTimer @ 0x0206cccc (284 bytes)                           */
/* Searches action trigger table for matching event type and increments     */
/* occurrence counter. If not found, allocates a new slot.                  */
/* Args: r0=player, r1=callback, r2=event_type                             */
/* Returns: matched/allocated slot index                                    */
/* ======================================================================== */
u32 Action_DecrementTimer(PhysicsPlayer *player, void *callback, u32 event_type) {
    extern s8 g_trigger_search_idx;
    extern s8 g_trigger_search_max;
    extern u16 g_trigger_table_types[];
    extern u16 g_trigger_table_counts[];

    s8 idx, max_idx;
    s32 match_idx = -1;

    /* If high nibble of event_type is set, skip direct search */
    if (!(event_type & 0xF000)) {
        idx = g_trigger_search_idx;
        max_idx = g_trigger_search_max;

        if (idx < max_idx) {
            /* Linear search for matching event type */
            while (idx < max_idx) {
                s32 tidx = (s32)idx;
                if (event_type == g_trigger_table_types[tidx]) {
                    /* Found: increment counter and return */
                    g_trigger_table_counts[tidx]++;
                    return (u32)idx;
                }
                idx++;
            }
        }
    }

    /* Not found or high-nibble event: find free slot */
    idx = g_trigger_search_idx;
    max_idx = g_trigger_search_max;

    if (idx >= max_idx) {
        return (u32)(max_idx > 0 ? max_idx - 1 : 0);
    }

    /* Search for unused slot */
    while (idx < max_idx) {
        s32 tidx = (s32)idx;
        if (g_trigger_table_types[tidx + 1] == 0) {
            /* Free slot found */
            match_idx = tidx;
            break;
        }
        idx++;
    }

    if (match_idx < 0) {
        return (u32)(max_idx > 0 ? max_idx - 1 : 0);
    }

    /* Store event type in table */
    g_trigger_table_types[match_idx] = (u16)event_type;

    /* Initialize the trigger entry via external helper */
    ActionTrigger_Setup(player, callback, (u32)match_idx, 0);
    ActionTrigger_Setup(player, callback, (u32)match_idx, 1);

    return (u32)match_idx;
}
