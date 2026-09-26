/* Movement and physics functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 313-421
 *
 * ARM946E-S (ARMv5TE) - ARM mode (not Thumb despite NDS convention)
 * Decompiled from binary at known addresses. */

#include "nds_types.h"
#include "player.h"

/* ======================================================================== */
/* External function declarations                                            */
/* ======================================================================== */

/* Unknown helper at 0x020693f0 - processes slope input acceleration.
 * Args: r0=ptr to s16 value (input/output), r1=slope_byte or ptr
 * Purpose: Applies slope-based modification to velocity component */
extern void SlopeInput_Process(s16 *value, u32 slope_param);

/* Unknown helper at 0x02068df8 - angle/velocity calculation.
 * Args: r0=angle?, r1=scaled_value
 * Returns: computed velocity component */
extern s32 AngleVelocity_Calc(s32 angle, s32 scaled_value);

/* Unknown helper at 0x02065ea8 - collision init/setup.
 * Args: r0=player
 * Returns: flags word used for collision setup */
extern u32 Collision_Setup(PhysicsPlayer *player);

/* collision_dispatcher @ 0x02066010 (160 bytes)
 * Dispatches collision test at given position.
 * Args: r0=player, r1=pos_x, r2=pos_y, r3=collision_flags,
 *       sp[0]=direction, sp[4]=?, sp[8]=?
 * Returns: signed collision distance */
extern s32 Collision_Dispatcher(PhysicsPlayer *player, s32 pos_x, s32 pos_y,
                                u32 flags, u32 dir, u32 p4, u32 p5);

/* Unknown at 0x02065624 - ground slope check?
 * Args: r0=player
 * Returns: non-zero if special ground condition */
extern s32 GroundSlope_Check(PhysicsPlayer *player);

/* Unknown at 0x020660b0 - update collision angle.
 * Args: r0=player, r1=angle_byte
 * Purpose: Updates prev_angle based on collision result */
extern void CollisionAngle_Update(PhysicsPlayer *player, u8 angle);

/* entity_aabb_sweep @ 0x02067670 (936 bytes)
 * Sweeps AABB for entity collision.
 * Args: r0=player, r1=pos_x, r2=pos_y, r3=flags,
 *       sp[0]=direction, sp[4]=?, sp[8]=?
 * Returns: signed collision distance */
extern s32 EntityAABB_Sweep(PhysicsPlayer *player, s32 pos_x, s32 pos_y,
                            u32 flags, u32 dir, u32 p4, u32 p5);

/* Unknown at 0x02066a9c - secondary collision check.
 * Args: r0=player, r1=pos_x, r2=pos_y, r3=flags,
 *       sp[0]=?, sp[4]=?
 * Returns: collision result */
extern s32 Collision_SecondaryCheck(PhysicsPlayer *player, s32 pos_x,
                                    s32 pos_y, u32 flags, u32 p4, u32 p5);

/* Unknown at 0x02068b04 - angle interpolation.
 * Args: r0=current_angle, r1=target_angle, r2=step
 * Returns: interpolated angle */
extern s16 Angle_Interpolate(s16 current, s16 target, s32 step);

/* ======================================================================== */
/* External global declarations                                             */
/* ======================================================================== */

/* Speed factor loaded from literal pool at 0x2069808 */
extern s16 g_movement_speed_factor;

/* Direction lookup table loaded from literal pool at 0x206980c/0x2069810.
 * Indexed by (slope_byte << 8) >> 12, contains sin/cos pairs */
extern const s16 g_slope_direction_table[];

/* Additional value from literal pool at 0x2069814 */
extern s16 g_movement_extra_factor;

/* Drift table at 0x02077944 */
extern const s8 g_drift_table[];

/* ======================================================================== */
/* Player_VelocityInjection @ 0x0206919c (128 bytes)                        */
/* Injects velocity from bound object (springs/platforms) into player.      */
/* Checks mover_binding entity, clamps partner velocity to accel_clamp,     */
/* and applies it to the player's raw input.                                */
/* Args: r0=PhysicsPlayer*                                                   */
/* Note: Function appears truncated in binary extraction (128 bytes,         */
/*       no return instruction). Decompilation covers visible instructions.  */
/* ======================================================================== */
void Player_VelocityInjection(PhysicsPlayer *player) {
    void *binding;
    void *partner;
    void *check_entity;
    u32 partner_flags;
    s16 accel_clamp;
    s16 partner_vel;
    s32 clamped;

    /* +0x130: mover_binding */
    binding = player->mover_binding;
    if (!binding) return;

    /* Check partner entity at binding+4 */
    partner = *(void **)((u8 *)binding + 4);
    if (partner) {
        /* +0x54: control_flags of bound entity */
        partner_flags = *(u32 *)((u8 *)partner + 0x54);
        if (partner_flags & PLAYER_CTRL_BUSY) {
            /* Clear the binding if partner is busy */
            *(void **)((u8 *)binding + 4) = NULL;
        }
    }

    /* Reload binding, check target entity at binding+8 */
    binding = player->mover_binding;
    partner = *(void **)((u8 *)binding + 8);
    if (!partner) return;

    /* Verify target differs from source */
    check_entity = *(void **)((u8 *)binding + 4);
    if (partner == check_entity) return;

    /* Check partner state_flags bit 25 */
    partner_flags = *(u32 *)((u8 *)partner + 0x58);
    if (!(partner_flags & (1 << 25))) return;

    /* Check player state_flags bit 26 */
    if (!(player->state_flags & (1 << 26))) return;

    /* Clamp partner velocity to accel_clamp */
    accel_clamp = *(s16 *)((u8 *)partner + 0x48);  /* partner->accel_clamp */
    partner_vel = *(s16 *)((u8 *)partner + 0x28);   /* partner->input_vel_x */
    clamped = -accel_clamp;

    if (partner_vel < (s32)(-accel_clamp)) {
        /* Already below minimum */
    } else if (partner_vel > (s32)accel_clamp) {
        clamped = accel_clamp;
    } else {
        clamped = partner_vel;
    }

    /* Load player's accel_clamp (function continues beyond extraction) */
    accel_clamp = player->accel_clamp;
    (void)clamped;
    (void)accel_clamp;
}

/* ======================================================================== */
/* Player_MovementAccelHandler @ 0x020694b0 (856 bytes)                     */
/* Main acceleration/velocity processing. Handles input scaling, gravity,   */
/* terminal velocity, slope acceleration, and velocity-to-position apply.   */
/* Args: r0=PhysicsPlayer*                                                   */
/* ======================================================================== */
void Player_MovementAccelHandler(PhysicsPlayer *player) {
    s16 stack_inputs[2];     /* [sp+0] saved input_x, [sp+2] saved input_y */
    s32 pos_backup[3];       /* target position backup */
    s32 result_flags = 0;
    s32 extra_x = 0, extra_y = 0;
    s16 gravity_effect;
    s16 terminal;
    u32 state;
    s16 input_x, input_y;
    s16 accel_factor;
    s16 raw_z;
    s32 slope_index;
    s16 table_val_x, table_val_y;

    /* Save raw inputs to stack */
    input_x = player->raw_input_x;    /* +0x36 */
    input_y = player->raw_input_y;    /* +0x38 */
    stack_inputs[0] = input_x;
    stack_inputs[1] = input_y;

    /* Backup position as target */
    player->target_x = player->pos_x;
    player->target_y = player->pos_y;
    player->target_z = player->pos_z;

    /* Check state_flags bit 28: if set, clear all raw inputs */
    state = player->state_flags;
    if (state & 0x10000000) {
        player->raw_input_x = 0;
        player->raw_input_y = 0;
        player->raw_input_z = 0;
    }

    /* If there's any raw input and slope_byte is non-zero, process slope */
    if (stack_inputs[0] != 0 || stack_inputs[1] != 0) {
        u8 slope_byte = ((u8 *)player)[5];
        if (slope_byte != 0) {
            SlopeInput_Process(&stack_inputs[0], (u32)&stack_inputs[1]);
        }
    }

    /* Check action_timer low nibble for drift table scaling */
    if (player->action_timer & 0x0F) {
        /* Scale raw inputs by speed factor */
        accel_factor = g_movement_speed_factor;
        player->input_vel_x = (s16)((stack_inputs[0] * accel_factor) >> 8);
        player->input_vel_y = (s16)((stack_inputs[1] * accel_factor) >> 8);
        raw_z = player->raw_input_z;
        player->input_vel_z = (s16)((raw_z * accel_factor) >> 8);
        goto apply_velocity;
    }

    /* === Airborne physics (when NOT grounded) === */
    state = player->state_flags;
    if (state & PLAYER_STATE_GROUNDED) goto apply_gravity;

    /* Apply gravity when bit 7 is set */
    if (state & 0x80) {
        gravity_effect = player->gravity;
        accel_factor = g_movement_speed_factor;
        s16 vy = player->applied_vel_y;
        vy = (s16)(vy + ((gravity_effect * accel_factor) >> 8));
        player->applied_vel_y = vy;
    }

    /* Terminal velocity clamp */
    if (state & 0x80) {
        terminal = player->terminal_vel;
        if (player->applied_vel_y > terminal) {
            player->applied_vel_y = terminal;
        }
    }

apply_gravity:
    /* === Slope acceleration processing (when state_flags bit 6 set) === */
    state = player->state_flags;
    if (!(state & 0x40)) goto compute_velocity;

    if (state & 0x20000) {
        /* Extra velocity mode */
        if (player->extra_vel != 0 || (state & 0x40000)) {
            u8 dir_byte = ((u8 *)player)[4];   /* direction byte */
            u8 slope_byte = ((u8 *)player)[1];  /* slope byte */
            s32 combined = (slope_byte + dir_byte) & 0xFF;
            if (combined < (s32)(dir_byte * 2)) {
                goto compute_velocity;
            }

            if (slope_byte != 0) {
                /* Use slope_byte to index direction table */
                slope_index = (s32)(((u32)(slope_byte << 24) >> 16) & 0xFFFF);
                slope_index = (slope_index >> 4);
                s16 slope_accel_x = player->slope_accel_x;
                s16 table_val = g_slope_direction_table[slope_index];
                s16 slope_accel_y = player->slope_accel_y;
                s32 computed = (slope_accel_x * table_val) >> 12;
                player->extra_vel = (s16)AngleVelocity_Calc(computed, slope_accel_y);
            } else {
                /* Inverse slope processing */
                slope_index = 0;
                s16 slope_accel_x = player->slope_accel_x;
                s16 table_val = g_slope_direction_table[0];
                s16 slope_accel_y = player->slope_accel_y;
                s32 computed = ((-slope_accel_x) * table_val) >> 12;
                player->extra_vel = (s16)AngleVelocity_Calc(computed, slope_accel_y);
            }
        }
    }

    /* === Direction-based velocity decomposition === */
    if (!(state & 0x8000)) {
        u8 slope_byte = ((u8 *)player)[1];
        slope_index = (s32)(((u32)(slope_byte << 24) >> 16) & 0xFFFF);
        slope_index = (slope_index >> 4);

        /* Look up X and Y components from direction table */
        s32 idx_x = slope_index * 2;
        s32 idx_y = idx_x + 1;
        s16 extra_vel = player->extra_vel;
        table_val_x = g_slope_direction_table[idx_x];
        table_val_y = g_slope_direction_table[idx_y];

        extra_x = (extra_vel * table_val_x) << 4;
        extra_y = (extra_vel * table_val_y) << 4;
        extra_x >>= 16;
        extra_y >>= 16;
    }

compute_velocity:
    /* === Final velocity computation === */
    state = player->state_flags;
    if (state & 0x8000000) {
        /* Special velocity mode (bit 27) */
        s16 saved_vx = player->saved_vel_x;
        accel_factor = g_movement_speed_factor;
        s16 input_x_saved = stack_inputs[0];

        s32 sum_x = saved_vx + extra_y + input_x_saved;
        player->input_vel_x = (s16)((accel_factor * sum_x) >> 8);

        s16 vy = player->applied_vel_y;
        s16 input_y_saved = stack_inputs[1];
        s32 sum_y = vy + extra_x + input_y_saved;
        player->input_vel_y = (s16)((accel_factor * sum_y) >> 8);
    } else {
        /* Regular velocity mode */
        s16 saved_vx = player->saved_vel_x;
        s16 input_x_saved = stack_inputs[0];
        s16 extra_factor = g_movement_extra_factor;
        accel_factor = g_movement_speed_factor;

        s32 sum_x = extra_factor + saved_vx + extra_y + input_x_saved;
        player->input_vel_x = (s16)((accel_factor * sum_x) >> 8);

        s16 vy = player->applied_vel_y;
        s16 input_y_saved = stack_inputs[1];
        s16 extra_factor_y = g_movement_extra_factor; /* [pc+2] = next halfword */
        s32 sum_y = extra_factor_y + vy + extra_x + input_y_saved;
        player->input_vel_y = (s16)((accel_factor * sum_y) >> 8);
    }

    /* Z-axis velocity */
    accel_factor = g_movement_speed_factor;
    raw_z = player->raw_input_z;
    s16 saved_vz = *(s16 *)((u8 *)player + 0x32);  /* saved_vel_z */
    player->input_vel_z = (s16)(((raw_z + saved_vz) * accel_factor) >> 8);

    /* Process slope input on Y component */
    SlopeInput_Process(&player->input_vel_y, (u32)((u8 *)player)[5]);

apply_velocity:
    /* Clear raw inputs */
    player->raw_input_x = 0;
    player->raw_input_y = 0;
    player->raw_input_z = 0;

    /* Apply input velocities to position */
    player->pos_x += player->input_vel_x;
    player->pos_y += player->input_vel_y;
    player->pos_z += player->input_vel_z;
}

/* ======================================================================== */
/* Player_MovementCollisionResolution @ 0x020657dc (1740 bytes)             */
/* Resolves movement against collision geometry. Tests collision at new     */
/* position, resolves penetration, and updates ground/wall state flags.     */
/* Args: r0=PhysicsPlayer*, r1=resolve_mode, r2=input_velocity_y           */
/* ======================================================================== */
void Player_MovementCollisionResolution(PhysicsPlayer *player, s32 resolve_mode,
                                        s32 input_vy) {
    s32 pos_x, pos_y;
    u32 state_flags;
    s32 collision_result;
    s32 collision_x, collision_y;
    s32 resolved_x, resolved_y;
    s32 pen_depth;
    u32 flags_save;
    u32 setup_flags;
    s32 direction_byte;
    s32 quadrant;        /* 0=down, 1=left, 2=up, 3=right */
    u8 margin_l, margin_r, margin_t, margin_b;
    s32 margin_x, margin_y;
    s32 resolved_neg_x, resolved_neg_y;
    s32 sign_x, sign_y;
    s32 test_flags;
    s32 temp;
    s32 resolve_axis;
    s32 pen_x, pen_y;
    u8 angle_result;
    s32 sweep_result;
    s32 vel_x_check;
    u32 state_check;

    collision_x = 0;
    flags_save = 0;
    pos_x = 0;

    /* Setup collision */
    setup_flags = Collision_Setup(player);

    /* Get direction byte and compute quadrant */
    direction_byte = ((u8 *)player)[1];
    quadrant = ((direction_byte + 32) & 0xC0) >> 6;

    pos_x = player->pos_x >> 8;
    pos_y = player->pos_y >> 8;

    resolve_axis = 64;  /* default margin */
    temp = 0;

    /* Quadrant-based margin adjustment */
    if (quadrant == 2) {
        resolve_axis = (resolve_axis + 128) & 0xFF;
    }

    /* Handle resolve_mode direction */
    if (input_vy > 0) {
        resolve_axis = (-resolve_axis) & 0xFF;
        temp = 1;
    } else {
        /* Check grounded state */
        if (!(player->status & 1)) {
            resolve_axis = (-resolve_axis) & 0xFF;
            temp = 1;
        }
    }

    /* Apply slope_byte offset */
    {
        u8 slope_byte = ((u8 *)player)[5];
        if (slope_byte != 0) {
            resolve_axis = (resolve_axis + slope_byte) & 0xFF;
            if (input_vy > 0) {
                resolve_axis = (-resolve_axis) & 0xFF;
                temp = 0;
            }
        }
    }

    /* Check state_flags bit 4 */
    state_flags = player->state_flags;
    if (!(state_flags & 0x10)) {
        direction_byte = ((u8 *)player)[1];
        pos_x += resolve_axis;
    }

    /* Ground slope check */
    collision_result = GroundSlope_Check(player);
    if (collision_result != 0) {
        resolve_axis = 6;
    }

    /* Compute quadrant from resolved angle */
    {
        s32 resolved_angle = ((resolve_axis + 32) & 0xC0) >> 6;
        test_flags = resolved_angle;
    }

    /* Switch on resolved quadrant for margin computation */
    switch (test_flags) {
    case 0:  /* Down-facing surface */
        margin_l = ((u8 *)player)[0x66] + 2;  /* margin_t */
        margin_r = ((u8 *)player)[0x67] - 4;  /* margin_b */
        margin_t = ((u8 *)player)[0x65] + 4;  /* margin_r */
        resolved_x = margin_l;
        resolved_neg_x = margin_r;
        resolved_y = margin_t;
        margin_x = resolved_x;
        margin_y = resolved_y;
        resolve_axis = 2;
        break;

    case 1:  /* Left-facing surface */
        margin_l = ((u8 *)player)[0x67] - 4;
        margin_r = ((u8 *)player)[0x64] - 2;  /* margin_l */
        margin_t = ((u8 *)player)[0x65] + resolve_axis;
        resolved_neg_x = margin_l;
        resolved_y = margin_t;
        sign_x = margin_r;
        sign_y = margin_t;

        /* Flip if direction reversed */
        if (temp != 0) {
            resolved_neg_x = -resolved_neg_x;
            sign_x = -sign_x;
        }
        resolve_axis = 1;
        break;

    case 2:  /* Right-facing surface */
        margin_l = ((u8 *)player)[0x64] - 2;
        margin_r = ((u8 *)player)[0x67] - 4;
        margin_t = ((u8 *)player)[0x65] + 4;
        resolved_x = margin_l;
        resolved_neg_x = margin_r;
        resolved_y = margin_t;
        margin_x = resolved_x;
        margin_y = resolved_y;
        resolve_axis = 3;
        break;

    case 3:  /* Up-facing surface */
        margin_l = ((u8 *)player)[0x66] + 2;
        margin_r = ((u8 *)player)[0x67] - 4;
        margin_t = ((u8 *)player)[0x65];
        resolved_neg_x = margin_l;
        resolved_y = margin_t;
        sign_x = margin_r;
        sign_y = margin_t;

        /* Negate if input velocity positive */
        if (input_vy > 0) {
            resolved_neg_x = -resolved_neg_x;
            sign_x = -sign_x;
        }
        resolve_axis = 0;
        break;
    }

    /* === Primary collision test === */
    {
        u32 test_col_flags = (setup_flags | 0x80) & 0xFF;
        collision_result = Collision_Dispatcher(
            player, pos_x + resolved_y, pos_y + margin_x,
            test_col_flags, 0, 0, resolve_axis);
        s16 col_result_primary = (s16)(collision_result << 24) >> 24;

        /* Secondary collision test at alternate position */
        s32 alt_result = Collision_Dispatcher(
            player, pos_x + margin_y, pos_y + sign_x,
            test_col_flags, 0, 0, resolve_axis);
        s16 col_result_secondary = (s16)(alt_result << 24) >> 24;

        /* Take minimum penetration depth */
        if (col_result_primary < col_result_secondary) {
            col_result_secondary = col_result_primary;
        }

        pen_depth = col_result_secondary;
    }

    /* === Resolve penetration if positive === */
    if (pen_depth > 0) {
        if (resolve_mode != 0) {
            /* Set state_flags bit 2 */
            state_flags = player->state_flags;
            player->state_flags = state_flags | PLAYER_STATE_IMPACT;

            /* Check state_flags bit 14 */
            if (!(player->state_flags & 0x4000)) {
                /* Direction-based velocity zeroing */
                if (resolve_axis == 1 && input_vy < 0) {
                    if (player->input_vel_x < 0) {
                        player->extra_vel = 0;
                        player->applied_vel_x = 0;
                    }
                }
                if (resolve_axis == 0 && input_vy > 0) {
                    if (player->input_vel_x > 0) {
                        player->extra_vel = 0;
                        player->applied_vel_x = 0;
                    }
                }
            }

            /* Check if axis is 0 or 1 (vertical/horizontal) */
            {
                u32 axis_check = (resolve_axis + 254) & 0xFF;
                if (axis_check <= 1) {
                    if (!(player->state_flags & 0x4000)) {
                        player->applied_vel_y = 0;
                        player->extra_vel = 0;
                    }
                }
            }
        }
    }

    /* === Apply penetration resolution to position === */
    {
        s32 new_angle = ((resolve_axis + 128) & 0xFF);
        new_angle = ((new_angle + 32) & 0xC0) >> 6;

        switch (new_angle) {
        case 0: /* Resolve downward */
            pos_y += pen_depth;
            break;
        case 1: /* Resolve leftward */
            pos_x -= pen_depth;
            break;
        case 2: /* Resolve upward */
            pos_y -= pen_depth;
            break;
        case 3: /* Resolve rightward */
            pos_x += pen_depth;
            break;
        }
    }

    /* === Secondary collision pass (mirrored) === */
    {
        u32 test_col_flags2 = (setup_flags | 0x80) & 0xFF;
        collision_result = Collision_Dispatcher(
            player, pos_x + resolved_y, pos_y + margin_x,
            test_col_flags2, 0, 0, resolve_axis);
        sweep_result = (s16)(collision_result << 24) >> 24;

        s32 alt_result2 = Collision_Dispatcher(
            player, pos_x + margin_y, pos_y + sign_x,
            test_col_flags2, 0, 0, resolve_axis);
        s16 col2 = (s16)(alt_result2 << 24) >> 24;

        if (sweep_result < col2) {
            col2 = sweep_result;
        }

        pen_depth = col2;
    }

    if (pen_depth > 0) {
        if (resolve_mode != 0) {
            /* Set impact flag */
            state_flags = player->state_flags;
            if (pen_depth < 0) {
                player->state_flags = state_flags | 0x8;
            }

            /* Direction-based velocity zeroing */
            if (resolve_axis == 1 && input_vy < 0) {
                player->extra_vel = 0;
            }
            if (resolve_axis == 0 && input_vy > 0) {
                player->extra_vel = 0;
            }

            /* Check state_flags bit 14 */
            if (!(player->state_flags & 0x4000)) {
                if (resolve_axis == 1) {
                    if (player->applied_vel_x < 0) {
                        player->applied_vel_x = 0;
                    }
                }
                if (resolve_axis == 0) {
                    if (player->applied_vel_x > 0) {
                        player->applied_vel_x = 0;
                    }
                }
            }
        }
    }

    /* === Update position from resolved coordinates === */
    if (resolve_mode != 0) {
        /* Early return - don't update position */
        return;
    }

    /* Convert fixed-point: new_pos = pos - (pos>>8 - resolved) << 8 */
    {
        s32 current_pos_x = player->pos_x;
        s32 delta_x = (current_pos_x >> 8) - pos_x;
        player->pos_x = current_pos_x - (delta_x << 8);

        s32 current_pos_y = player->pos_y;
        s32 delta_y = (current_pos_y >> 8) - pos_y;
        player->pos_y = current_pos_y - (delta_y << 8);
    }
}

/* ======================================================================== */
/* Player_GroundCollisionSlopeSnap @ 0x020648c4 (3416 bytes)               */
/* Ground collision detection with slope snapping. Tests collision below    */
/* player, resolves ground contact, and snaps position to terrain surface.  */
/* Also handles slope transitions and ceiling contact.                      */
/* Args: r0=PhysicsPlayer*                                                   */
/* Returns: s32 ground_contact_depth (negative = ground found)              */
/* ======================================================================== */
s32 Player_GroundCollisionSlopeSnap(PhysicsPlayer *player) {
    u32 saved_binding;
    u32 setup_flags;
    s32 pos_x, pos_y;
    s32 ground_depth;
    s32 ceiling_depth;
    u8 angle_byte;
    u8 resolved_angle;
    s32 direction_byte;
    s32 slope_quadrant;
    s32 collision_x, collision_y;
    s32 test_flags;
    s32 margin_down, margin_up;
    s32 margin_left, margin_right;
    s32 temp_x, temp_y;
    s32 neg_margin_x, neg_margin_y;
    s32 sweep_flags;
    s32 pen_a, pen_b;
    s16 vel_y_save;
    s32 vel_y_input;
    s32 resolved_quadrant;
    s32 ground_result;
    s32 ceiling_result;
    s32 final_depth;
    s32 state_flags;
    s32 floor_offset;
    s32 i;
    s32 step_result;
    s32 test_x, test_y;
    s32 min_pen;
    s32 snap_amount;
    s32 effective_dir;

    /* Save mover_binding and clear it */
    saved_binding = (u32)player->mover_binding;

    /* Setup collision system */
    setup_flags = Collision_Setup(player);

    /* Load position (fixed-point >> 8 = pixel) */
    pos_x = player->pos_x >> 8;
    pos_y = player->pos_y >> 8;

    /* Load direction byte and slope parameters */
    direction_byte = ((u8 *)player)[1];
    angle_byte = direction_byte;
    resolved_angle = direction_byte;

    /* Check state_flags bit 4 */
    state_flags = player->state_flags;
    if (state_flags & 0x10) {
        floor_offset = 0;
    } else {
        /* Compute quadrant from direction */
        floor_offset = ((direction_byte + 32) & 0xC0) >> 6;
    }

    /* Apply slope_byte offset to quadrant */
    {
        u8 slope_byte = ((u8 *)player)[5];
        if (slope_byte != 0) {
            s32 slope_adjust = ((slope_byte + 32) & 0xC0) >> 6;
            floor_offset = (floor_offset + slope_adjust) & 3;
        }
    }

    /* === Compute collision margins based on quadrant === */
    switch (floor_offset) {
    case 0:  /* Downward */
        margin_left = ((s8 *)player)[0x66] + 2;
        margin_right = ((s8 *)player)[0x67] - 4;
        margin_down = ((s8 *)player)[0x65] - 2;
        margin_up = ((s8 *)player)[0x64] - 2;

        neg_margin_x = margin_right;
        neg_margin_y = margin_left;
        vel_y_save = player->input_vel_x;
        ground_result = 0;
        sweep_flags = -5;
        ceiling_result = 2;
        break;

    case 1:  /* Leftward */
        margin_left = ((s8 *)player)[0x67];
        margin_right = ((s8 *)player)[0x64] - 2;
        margin_down = ((s8 *)player)[0x66] + 2;
        margin_up = ((s8 *)player)[0x65] + 4;

        neg_margin_x = (-margin_left) & 0xFF;
        neg_margin_x = (s8)neg_margin_x;

        temp_x = margin_right - 2;
        neg_margin_y = margin_up;
        test_flags = margin_down;

        if (margin_down < 0) {
            temp_x = (-margin_down) & 0xFF;
            temp_x = (s8)temp_x;
        }
        temp_x = (s8)temp_x;

        vel_y_save = (s16)(-player->input_vel_x);
        ground_result = (s16)(-player->input_vel_y);
        sweep_flags = 0;
        ceiling_result = 1;
        break;

    case 2:  /* Upward */
        margin_left = ((s8 *)player)[0x67];
        margin_right = ((s8 *)player)[0x64] - 2;
        margin_down = ((s8 *)player)[0x66] + 2;
        margin_up = ((s8 *)player)[0x65] + 4;

        neg_margin_x = margin_right;
        neg_margin_y = margin_left;
        vel_y_save = (s16)(-player->input_vel_x);
        ground_result = (s16)(-player->input_vel_y);
        sweep_flags = 5;
        ceiling_result = 3;
        break;

    case 3:  /* Rightward */
        margin_left = ((s8 *)player)[0x64] - 2;
        margin_right = ((s8 *)player)[0x66] + 2;
        margin_down = ((s8 *)player)[0x65] - 2;
        margin_up = ((s8 *)player)[0x67];

        temp_x = margin_left - 2;
        neg_margin_x = margin_right + 2;
        neg_margin_y = margin_up;

        if (margin_left < 0) {
            temp_x = (-margin_left) & 0xFF;
            temp_x = (s8)temp_x;
        }

        vel_y_save = player->input_vel_x;
        ground_result = 0;
        sweep_flags = 0;
        ceiling_result = 0;
        break;
    }

    /* === Determine collision direction from state_flags === */
    test_flags = setup_flags;
    if (setup_flags & 0x400000) {
        /* Special case: check floor/ceiling based on resolve_axis */
        if (ceiling_result == 3 || ceiling_result == 2) {
            player->state_flags |= 0x100000;
        } else if (player->prev_angle & 8) {
            if (!(player->status & 8)) {
                player->state_flags |= 0x100000;
            } else {
                player->state_flags &= ~0x100000;
            }
        } else {
            player->state_flags &= ~0x100000;
        }
    } else {
        /* Normal collision: test both directions */
        s32 col_flags = (setup_flags & ~0x80) & 0xFF;

        /* Test in primary direction */
        ground_depth = Collision_Dispatcher(
            player, pos_x + margin_down + sweep_flags,
            pos_y + margin_up,
            col_flags, ceiling_result, 0, 0);
        ground_depth = (s8)ground_depth;

        /* Test in secondary direction */
        ceiling_depth = Collision_Dispatcher(
            player, pos_x + neg_margin_y + sweep_flags,
            pos_y + neg_margin_x,
            col_flags, ceiling_result, 0, 0);
        ceiling_depth = (s8)ceiling_depth;

        /* Take minimum depth */
        if (ground_depth < ceiling_depth) {
            ceiling_depth = ground_depth;
        }

        /* Store ground contact flag */
        if (ceiling_depth >= 0) {
            player->state_flags |= 0x100000;
        } else {
            player->state_flags &= ~0x100000;
        }
    }

    /* Restore mover_binding */
    player->mover_binding = (void *)saved_binding;

    /* === Primary ground collision test === */
    {
        s32 test_col = (setup_flags | 0x80) & 0xFF;
        collision_x = pos_x + margin_down;
        collision_y = pos_y + neg_margin_x;

        ground_depth = Collision_Dispatcher(
            player, collision_x, collision_y,
            test_col, ceiling_result, &resolved_angle, &angle_byte);
        ground_depth = (s8)ground_depth;

        CollisionAngle_Update(player, angle_byte);

        /* Secondary test */
        collision_x = pos_x + neg_margin_y;
        collision_y = pos_y + neg_margin_x;

        ceiling_depth = Collision_Dispatcher(
            player, collision_x, collision_y,
            test_col, ceiling_result, &angle_byte, &resolved_angle);
        ceiling_depth = (s8)ceiling_depth;

        CollisionAngle_Update(player, angle_byte);
    }

    /* === Check for slope snap conditions === */
    state_flags = player->state_flags;
    if (state_flags & 0x200000) {
        /* Already on ground - check for step-up */
        if (!(state_flags & 0x200)) {
            /* Check if step-up is possible */
            if (margin_down > neg_margin_y) {
                s32 step_range = margin_down - neg_margin_y - 1;
                if (step_range > 1) {
                    /* Test step-up at each pixel */
                    s32 step_x = pos_x + neg_margin_x;
                    s32 step_y = pos_y + margin_down;

                    for (i = 1; i < step_range; i++) {
                        step_result = EntityAABB_Sweep(
                            player, step_x + i, step_y,
                            setup_flags, 0, 0, ceiling_result);
                        step_result = (s8)step_result;

                        if (step_result < ground_depth) {
                            ground_depth = step_result;
                            angle_byte = ((u8 *)player)[0x66];
                            ((u8 *)player)[0x64] = angle_byte;
                        }
                    }
                }
            } else {
                /* Test step-down */
                s32 step_range = neg_margin_y - margin_down - 1;
                if (step_range > 1) {
                    s32 step_x = pos_x + margin_down;
                    s32 step_y = pos_y + margin_up;

                    for (i = 1; i < step_range; i++) {
                        step_result = EntityAABB_Sweep(
                            player, step_x + i, step_y,
                            setup_flags, 0, 0, ceiling_result);
                        step_result = (s8)step_result;

                        if (step_result < ground_depth) {
                            ground_depth = step_result;
                            ((u8 *)player)[0x64] = ((u8 *)player)[0x66];
                        }
                    }
                }
            }
        }
    }

    /* === Check for ceiling contact === */
    {
        s32 ceil_test_x, ceil_test_y;
        s32 ceil_range;

        if (margin_down < neg_margin_y) {
            /* Test ceiling direction */
            if (neg_margin_y > 0) {
                ceil_range = neg_margin_y;
            } else {
                ceil_range = -neg_margin_y;
            }
            ceil_range += margin_down;

            ceil_test_x = pos_x + (ceil_range >> 1);
            ceil_test_y = pos_y + neg_margin_x;

            ceiling_depth = Collision_Dispatcher(
                player, ceil_test_x, ceil_test_y,
                setup_flags, ceiling_result, &angle_byte, &resolved_angle);
        } else {
            if (margin_down > 0) {
                ceil_range = margin_down;
            } else {
                ceil_range = -margin_down;
            }
            ceil_range += neg_margin_y;

            ceil_test_x = pos_x + margin_down + (ceil_range >> 1);
            ceil_test_y = pos_y + margin_up;

            ceiling_depth = Collision_Dispatcher(
                player, ceil_test_x, ceil_test_y,
                setup_flags, ceiling_result, &angle_byte, &resolved_angle);
        }
    }

    CollisionAngle_Update(player, angle_byte);

    /* === Wall collision special case (prev_angle bit 3, state bit 4) === */
    if ((player->prev_angle & 8) && (player->state_flags & 0x10)) {
        if (vel_y_save > 0) {
            if (!(player->prev_angle & 1)) {
                /* Check if wall contact angle is within range */
                u8 test_angle = resolved_angle + ((u8 *)player)[5];
                test_angle &= 0xFF;
                if (test_angle >= 0x40 && test_angle <= 0xC0) {
                    ground_depth = 24;
                    ceiling_depth = 24;
                }
            }
        }
    }

    /* === Determine final ground contact depth === */
    if (ceiling_depth < ground_depth) {
        final_depth = ceiling_depth;
    } else {
        final_depth = ground_depth;
    }

    if (final_depth == 0) return 0;

    if (final_depth < 0) {
        /* Ground penetration found */
        state_flags = player->state_flags;
        if (state_flags & 0x10) {
            if (vel_y_save >= 0) goto apply_ground_resolution;
        }
        /* Set grounded flag */
        player->state_flags = state_flags | 1;

        /* Check penetration severity */
        if (final_depth < -12) {
            goto apply_ground_resolution;
        }

        if (player->state_flags & 1) {
            if (final_depth == -1) {
                /* Check state_flags bit 16 for special handling */
                if (player->state_flags & 0x10000) {
                    /* Multi-collision resolution */
                    s32 resolve_margin;
                    u8 resolve_dir;
                    u8 resolve_dir2;

                    /* Determine resolve direction from quadrant */
                    switch (ceiling_result) {
                    case 0: /* down */
                        resolve_margin = neg_margin_y + final_depth;
                        neg_margin_y += final_depth;
                        break;
                    case 2: /* up */
                        resolve_margin = margin_down - final_depth;
                        margin_down -= final_depth;
                        break;
                    case 3: /* right */
                        resolve_margin = neg_margin_y + final_depth;
                        neg_margin_y += final_depth;
                        break;
                    case 1: /* left */
                        resolve_margin = margin_down - final_depth;
                        margin_down -= final_depth;
                        break;
                    }

                    /* Test both directions for collision */
                    resolve_dir = resolve_margin;
                    resolve_dir2 = resolve_margin;

                    test_x = pos_x + margin_down;
                    test_y = pos_y + margin_up;
                    ground_depth = Collision_SecondaryCheck(
                        player, test_x, test_y, 0, 0, 0);

                    test_x = pos_x + neg_margin_y;
                    test_y = pos_y + margin_up;
                    ceiling_depth = Collision_SecondaryCheck(
                        player, test_x, test_y, 0, 0, 0);

                    if (ground_depth < ceiling_depth) {
                        ceiling_depth = ground_depth;
                    }

                    if (ceiling_depth == 1) goto apply_ground_resolution;

                    /* Apply resolution by direction */
                    switch (ceiling_result) {
                    case 0: pos_y += final_depth; break;
                    case 1: pos_x -= final_depth; break;
                    case 2: pos_y -= final_depth; break;
                    case 3: pos_x += final_depth; break;
                    }

                    /* Clear step-up flag */
                    player->state_flags &= ~0x10000;
                }
                goto apply_ground_resolution;
            }
        }

        /* Standard ground resolution */
        if (final_depth >= -12) {
            /* Set grounded flag */
            player->state_flags |= 1;

            /* Apply resolution by direction */
            switch (ceiling_result) {
            case 0: pos_y += final_depth; break;
            case 1: pos_x -= final_depth; break;
            case 2: pos_y -= final_depth; break;
            case 3: pos_x += final_depth; break;
            }

            /* Check for slope snap conditions */
            state_flags = player->state_flags;
            if (!(state_flags & 0x200) && !(state_flags & 0x40)) {
                if (player->partner == NULL) {
                    /* Additional ground collision verification */
                    test_x = pos_x + neg_margin_x;
                    test_y = pos_y + margin_down;
                    ground_depth = EntityAABB_Sweep(
                        player, test_x, test_y,
                        setup_flags, ceiling_result, 0, 0);

                    test_x = pos_x + neg_margin_y;
                    test_y = pos_y + margin_up;
                    ceiling_depth = EntityAABB_Sweep(
                        player, test_x, test_y,
                        setup_flags, ceiling_result, 0, 0);
                }
            }
        } else {
            /* Too deep - not grounded */
            player->state_flags &= ~1;
        }
    } else {
        /* No ground penetration */
        state_flags = player->state_flags;
        if (state_flags & 0x10) {
            if (player->applied_vel_y < 0) goto done;
        }
        /* Set grounded flag */
        player->state_flags = state_flags | 1;
    }

apply_ground_resolution:
    /* === Update ground contact angle === */
    state_flags = player->state_flags;
    if (state_flags & 1) {
        /* Grounded: update angle */
        if (!(player->prev_angle & 1) && (player->state_flags & 0x40)) {
            /* Check collision depth for angle selection */
            if (ground_depth >= ceiling_depth) {
                if (ground_depth > ceiling_depth) {
                    resolved_angle = angle_byte;
                }
            } else {
                resolved_angle = angle_byte;
            }

            /* Check if angle needs adjustment */
            {
                u8 player_dir = ((u8 *)player)[1];
                u8 slope_byte = ((u8 *)player)[5];
                u8 combined = (player_dir + slope_byte) & 0xFF;
                u8 diff1, diff2;

                diff1 = combined - resolved_angle;
                if (diff1 & 0x80) diff1 = -diff1;
                diff2 = combined - ((u8 *)player)[0x64];
                if (diff2 & 0x80) diff2 = -diff2;

                if (diff2 > diff1) {
                    resolved_angle = ((u8 *)player)[0x64];
                }
            }

            /* Apply slope_byte offset */
            {
                u8 slope_byte = ((u8 *)player)[5];
                if (slope_byte != 0) {
                    resolved_angle += slope_byte;
                }
            }

            /* Update angle based on state_flags */
            state_flags = player->state_flags;
            if (state_flags & 0x800000) {
                /* Direct angle write */
                ((u8 *)player)[1] = resolved_angle;
            } else {
                /* Interpolated angle update */
                u8 current_angle = ((u8 *)player)[1];
                s32 new_angle = Angle_Interpolate(
                    current_angle, resolved_angle, 1);
                ((u8 *)player)[1] = (u8)new_angle;
            }
        }

        /* === Process ground slope transitions === */
        state_flags = player->state_flags;
        if (!(state_flags & 0x4000)) {
            /* Compute effective direction */
            if (state_flags & 0x10) {
                effective_dir = 0;
            } else {
                effective_dir = ((u8 *)player)[1];
                effective_dir = ((effective_dir + 32) & 0xC0) >> 6;
            }

            /* Check slope transition thresholds */
            switch (effective_dir) {
            case 0: /* flat/down */
                /* No velocity modification needed */
                break;
            case 2: /* up-facing */
                /* Zero negative velocities */
                if (player->applied_vel_x < 0) {
                    player->applied_vel_x = 0;
                }
                break;
            case 1: /* left-facing */
                if (player->input_vel_x < 0) {
                    player->input_vel_x = 0;
                }
                break;
            case 3: /* right-facing */
                if (player->applied_vel_x > 0) {
                    player->applied_vel_x = 0;
                }
                break;
            }
        }

        /* Check state_flags bit 14 for special handling */
        if (player->state_flags & 0x4000) {
            goto done;
        }

        /* === Slope velocity transfer === */
        state_flags = player->state_flags;
        if (!(state_flags & 0x8000)) {
            /* Compute direction index */
            s32 dir_index;
            if (state_flags & 0x10) {
                dir_index = 0;
            } else {
                dir_index = ((u8 *)player)[1];
                dir_index = ((dir_index + 32) & 0xC0) >> 6;
            }

            /* Check if velocity exceeds threshold */
            if (player->input_vel_x > 0) {
                /* Check state_flags bit 17 */
                if (state_flags & 0x20000) {
                    /* Use direction table for velocity decomposition */
                    u8 slope_byte = ((u8 *)player)[1];
                    s32 table_idx = (s32)(((u32)(slope_byte << 24) >> 16) & 0xFFFF);
                    table_idx = (table_idx >> 4);
                    table_idx = table_idx * 2;

                    s16 table_x = g_slope_direction_table[table_idx];
                    s16 saved_vx = player->applied_vel_x;
                    s32 transfer = (saved_vx * table_x) << 4;

                    player->applied_vel_y += (s16)(transfer >> 16);
                }

                /* Clear input_vel_x */
                player->input_vel_x = 0;
            }
            /* Direction-specific zeroing */
            else if (effective_dir == 2) {
                if (player->applied_vel_x < 0) {
                    player->applied_vel_x = 0;
                }
            }
            else if (effective_dir == 1) {
                if (player->input_vel_x < 0) {
                    player->input_vel_x = 0;
                }
            }
            else if (effective_dir == 3) {
                if (player->applied_vel_x > 0) {
                    player->applied_vel_x = 0;
                }
            }
        }
    } else {
        /* Not grounded: restore mover_binding */
        player->mover_binding = (void *)saved_binding;
    }

    /* === Ceiling collision processing === */
    vel_y_input = player->input_vel_y;
    if (vel_y_input >= 256) goto done;

    /* Compute ceiling direction */
    switch (ceiling_result) {
    case 0: /* down-facing */
        margin_left = ((s8 *)player)[0x65] + 2;
        floor_offset = 3;
        neg_margin_x = margin_left;
        neg_margin_y = margin_left;
        break;
    case 2: /* up-facing */
        margin_left = ((s8 *)player)[0x66] - 2;
        floor_offset = 0;
        neg_margin_x = margin_left;
        neg_margin_y = margin_left;
        break;
    case 3: /* right-facing */
        margin_left = ((s8 *)player)[0x67] - 2;
        floor_offset = 2;
        neg_margin_x = margin_left;
        neg_margin_y = margin_left;
        break;
    case 1: /* left-facing */
        margin_left = ((s8 *)player)[0x64] + 2;
        floor_offset = 1;
        neg_margin_x = margin_left;
        neg_margin_y = margin_left;
        break;
    }

    /* Test ceiling collision */
    {
        u32 ceil_flags = (setup_flags | 0x80) & 0xFF;
        test_x = pos_x + neg_margin_x;
        test_y = pos_y + margin_left;

        ground_depth = Collision_Dispatcher(
            player, test_x, test_y,
            ceil_flags, floor_offset, 0, 0);
        ground_depth = (s8)ground_depth;

        test_x = pos_x + neg_margin_y;
        test_y = pos_y + margin_left;

        ceiling_depth = Collision_Dispatcher(
            player, test_x, test_y,
            ceil_flags, floor_offset, 0, 0);
        ceiling_depth = (s8)ceiling_depth;

        if (ground_depth < ceiling_depth) {
            ceiling_depth = ground_depth;
        }

        if (ceiling_depth > 0) {
            /* Ceiling penetration found */
            state_flags = player->state_flags;
            player->state_flags = state_flags | 2;

            /* Apply resolution */
            if (ceiling_depth >= -12) {
                switch (floor_offset) {
                case 0: pos_y += ceiling_depth; break;
                case 1: pos_x -= ceiling_depth; break;
                case 2: pos_y -= ceiling_depth; break;
                case 3: pos_x += ceiling_depth; break;
                }

                /* Check state_flags bit 14 */
                if (!(player->state_flags & 0x4000)) {
                    /* Direction-specific velocity zeroing */
                    if (floor_offset == 1 && vel_y_input < 0) {
                        player->extra_vel = 0;
                    }
                    if (floor_offset == 0 && vel_y_input > 0) {
                        player->extra_vel = 0;
                    }
                }
            }
        }
    }

done:
    /* Convert resolved position back to fixed-point */
    {
        s32 current_x = player->pos_x;
        s32 delta_x = (current_x >> 8) - pos_x;
        player->pos_x = current_x - (delta_x << 8);

        s32 current_y = player->pos_y;
        s32 delta_y = (current_y >> 8) - pos_y;
        player->pos_y = current_y - (delta_y << 8);
    }

    return ground_depth;
}

/* ======================================================================== */
/* Player_CharController @ 0x02065ee0 (184 bytes)                           */
/* Main character controller. Orchestrates collision detection, ground snap, */
/* and velocity resolution. Called from Player_PhysicsStep.                 */
/* Args: r0=PhysicsPlayer*                                                   */
/* ======================================================================== */
void Player_CharController(PhysicsPlayer *player) {
    s32 gravity;

    /* Clear state_flags bit 22 */
    player->state_flags &= ~PLAYER_STATE_X;

    /* If grounded (bit 0), set bit 22 */
    if (player->state_flags & PLAYER_STATE_GROUNDED) {
        player->state_flags |= PLAYER_STATE_X;
    }

    /* Clear low 4 bits of state_flags */
    player->state_flags &= ~0x0F;

    /* Clear prev_angle */
    player->prev_angle = 0;

    /* Save gravity value */
    gravity = player->gravity;

    /* Run collision resolution if bit 10 not set */
    if (!(player->state_flags & 0x400)) {
        Player_MovementCollisionResolution(player, gravity, 0);
    }

    /* Run ground collision if bit 11 not set */
    if (!(player->state_flags & 0x800)) {
        Player_GroundCollisionSlopeSnap(player);
    }

    /* If grounded, snap pos_y to grid (clear low 8 bits) */
    if (player->state_flags & PLAYER_STATE_GROUNDED) {
        player->pos_y &= ~0xFF;
    }

    /* If bit 10 not set, run collision resolution again with mode=1 */
    if (!(player->state_flags & 0x400)) {
        Player_MovementCollisionResolution(player, gravity, 1);
    }
}
