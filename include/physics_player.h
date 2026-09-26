/* PhysicsPlayer struct - 0x134 bytes, used by player subsystem.
 * Based on member access analysis of all player/*.c files. */

#ifndef PHYSICS_PLAYER_H
#define PHYSICS_PLAYER_H

#include "nds_types.h"

typedef struct PhysicsPlayer {
    /* +0x00 */ s32 pos_x;
    /* +0x04 */ s32 pos_y;
    /* +0x08 */ s32 pos_z;
    /* +0x0C */ s32 applied_vel_x;
    /* +0x10 */ s32 applied_vel_y;
    /* +0x14 */ s32 applied_vel_z;
    /* +0x18 */ s32 input_vel_x;
    /* +0x1C */ s32 input_vel_y;
    /* +0x20 */ s32 raw_input_x;
    /* +0x24 */ s32 raw_input_y;
    /* +0x28 */ s32 raw_input_z;
    /* +0x2C */ s32 target_x;
    /* +0x30 */ s32 target_y;
    /* +0x34 */ s32 target_z;
    /* +0x38 */ s32 extra_vel;
    /* +0x3C */ s32 terminal_vel;
    /* +0x40 */ s32 gravity;
    /* +0x44 */ s32 slope_accel_x;
    /* +0x48 */ s32 slope_accel_y;
    /* +0x4C */ u32 state_flags;
    /* +0x50 */ u32 control_flags;
    /* +0x54 */ u32 status;
    /* +0x58 */ u32 state_halfword;
    /* +0x5C */ u32 direction;
    /* +0x60 */ u32 wall_direction;
    /* +0x64 */ u32 action_state_a;
    /* +0x68 */ u32 action_state_b;
    /* +0x6C */ u32 action_state_c;
    /* +0x70 */ u32 action_timer;
    /* +0x74 */ u32 current_trick;
    /* +0x78 */ u32 trick_timer;
    /* +0x7C */ u32 trick_duration;
    /* +0x80 */ u32 trick_bonus;
    /* +0x84 */ u32 trick_rotation;
    /* +0x88 */ u32 slide_direction;
    /* +0x8C */ u32 slide_timer;
    /* +0x90 */ u32 roll_direction;
    /* +0x94 */ u32 roll_timer;
    /* +0x98 */ u32 dash_direction;
    /* +0x9C */ u32 dash_timer;
    /* +0xA0 */ u32 homing_timer;
    /* +0xA4 */ void *homing_target;
    /* +0xA8 */ u32 stomp_bounce;
    /* +0xAC */ u32 grind_direction;
    /* +0xB0 */ u32 grind_timer;
    /* +0xB4 */ u32 wall_jump_timer;
    /* +0xB8 */ u32 invuln_timer;
    /* +0xBC */ void *partner;
    /* +0xC0 */ void *partner_ref;
    /* +0xC4 */ void *object_binding;
    /* +0xC8 */ void *hitbox_link_fn;
    /* +0xCC */ void *state_change_fn;
    /* +0xD0 */ void *action_fn;
    /* +0xD4 */ void *action_fn_2;
    /* +0xD8 */ void *misc_fn_2;
    /* +0xDC */ u32 hitbox_group_a;
    /* +0xE0 */ u32 hitbox_group_b;
    /* +0xE4 */ u32 zone_id;
    /* +0xE8 */ u32 _pad_e8[19]; /* padding to 0x134 */
} PhysicsPlayer;

/* PhysicsPlayer state_flags bits */
#define PSF_ON_GROUND     (1 << 0)
#define PSF_IN_AIR        (1 << 1)
#define PSF_ROLLING       (1 << 2)
#define PSF_SLIDING       (1 << 3)
#define PSF_GRINDING      (1 << 4)
#define PSF_HOMING        (1 << 5)
#define PSF_STOMPING      (1 << 6)
#define PSF_DEAD          (1 << 7)
#define PSF_INVINCIBLE    (1 << 8)
#define PSF_SPEED_SHOES   (1 << 9)
#define PSF_SHIELDED      (1 << 10)
#define PSF_UNDERWATER    (1 << 11)

/* PhysicsPlayer control_flags bits */
#define PCF_JUMP_HELD     (1 << 0)
#define PCF_ATTACK_HELD   (1 << 1)
#define PCF_LEFT_HELD     (1 << 2)
#define PCF_RIGHT_HELD    (1 << 3)
#define PCF_UP_HELD       (1 << 4)
#define PCF_DOWN_HELD     (1 << 5)

#endif /* PHYSICS_PLAYER_H */