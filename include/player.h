#ifndef PLAYER_H
#define PLAYER_H

#include "nds_types.h"

/* Physics player entity struct (0x134 bytes)
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 325-400
 * Created by fcn.0204da44 with update 0x0206c3d0, destroy 0x0206bef4 */
typedef struct {
    void* self;                 /* +0x00: self ptr / link */
    void* parent;               /* +0x04: parent ptr */
    void (*update)(void*);      /* +0x08: update fn ptr */
    u16 flags0c;                /* +0x0c: halfword flags */
    s32 pos_x;                  /* +0x10: pos X (8:8 fixed) */
    s32 pos_y;                  /* +0x14: pos Y (8:8 fixed) */
    s32 pos_z;                  /* +0x18: pos Z (8:8 fixed) */
    s32 target_x;               /* +0x1c: target X (movement iterator) */
    s32 target_y;               /* +0x20: target Y */
    s32 target_z;               /* +0x24: target Z */
    s16 input_vel_x;            /* +0x28: input velocity X (signed halfword) */
    s16 input_vel_y;            /* +0x2a: input velocity Y */
    s16 input_vel_z;            /* +0x2c: input velocity Z */
    s16 applied_vel_x;          /* +0x2e: applied velocity X */
    s16 applied_vel_y;          /* +0x30: applied velocity Y */
    s16 extra_vel;              /* +0x34: extra velocity (impact/spring) */
    s16 raw_input_x;            /* +0x36: raw input X (per-frame scratch) */
    s16 raw_input_y;            /* +0x38: raw input Y */
    s16 raw_input_z;            /* +0x3a: raw input Z */
    s16 slope_accel_x;          /* +0x3c: slope-accel source X */
    s16 slope_accel_y;          /* +0x3e: slope-accel source Y */
    s16 gravity;                /* +0x40: gravity value */
    s16 terminal_vel;           /* +0x42: terminal/fall velocity clamp */
    s16 direction;              /* +0x44: facing direction (1=right, -1=left) */
    s16 accel_clamp;            /* +0x48: accel clamp value (spring/transport) */
    s16 hitbox_offset_x;        /* +0x4a: AABB/hitbox offset X */
    s16 hitbox_offset_y;        /* +0x4c: AABB/hitbox offset Y */
    s16 saved_vel_x;            /* +0x4e: saved velocity X */
    s16 saved_vel_y;            /* +0x50: saved velocity Y */
    s16 saved_vel_z;            /* +0x52: saved velocity Z */
    u16 control_flags;          /* +0x54: control flags (see below) */
    u32 state_flags;            /* +0x58: state flags (see below) */
    u16 status;                 /* +0x5c: halfword status */
    s16 prev_angle;             /* +0x5e: previous angle */
    s16 prev_angle_stored;      /* +0x60: prev angle stored by iterator */
    u16 action_timer;           /* +0x62: action timer (see below) */
    s8 margin_l;                /* +0x64: collision box margin L */
    s8 margin_r;                /* +0x65: margin R */
    s8 margin_t;                /* +0x66: margin T */
    s8 margin_b;                /* +0x67: margin B */
    void (*action_fn)(void*);   /* +0x68: action fn ptr */
    void (*action_fn_2)(void*); /* +0x6c: action fn ptr 2 */
    void (*action_fn_3)(void*); /* +0x70: action fn ptr 3 */
    u32 _pad74;                 /* +0x74: unknown */
    void (*misc_fn_1)(void*);   /* +0x78: misc fn ptr */
    void (*misc_fn_2)(void*);   /* +0x7c: misc fn ptr */
    void* object_binding;       /* +0x80: object binding ptr (springs/platforms) */
    void* partner;              /* +0x84: partner/companion entity ptr */
    void* partner_ref;          /* +0x88: object ptr (partner ref) */
    void* action_data;          /* +0x8c: action data ptr */
    u32 _pad90;                 /* +0x90: unknown */
    void* hitbox_group_a;       /* +0x94: hitbox group A ptr */
    u32 _pad98[2];              /* +0x98: unknown */
    void (*hitbox_link_fn)(void*); /* +0xa4: hitbox link fn */
    u16 trick_timer;            /* +0xa8: trick timer */
    u16 trick_duration;         /* +0xaa: trick duration */
    u16 trick_bonus;            /* +0xac: trick bonus */
    u16 trick_rotation;         /* +0xae: trick rotation */
    u16 current_trick;          /* +0xb0: current trick ID */
    u16 slide_direction;        /* +0xb2: slide direction */
    u16 slide_timer;            /* +0xb4: slide timer */
    u16 roll_direction;         /* +0xb6: roll direction */
    u16 roll_timer;             /* +0xb8: roll timer */
    u16 dash_direction;         /* +0xba: dash direction */
    u16 dash_timer;             /* +0xbc: dash timer */
    u16 homing_timer;           /* +0xbe: homing timer */
    void* homing_target;        /* +0xc0: homing target ptr */
    u32 stomp_bounce;           /* +0xc4: stomp bounce flag */
    u16 grind_direction;        /* +0xc8: grind direction */
    u16 grind_timer;            /* +0xca: grind timer */
    u16 wall_jump_timer;        /* +0xcc: wall jump timer */
    u16 invuln_timer;           /* +0xce: invulnerability timer */
    u16 wall_direction;         /* +0xd0: wall direction */
    u16 _padd2;                 /* +0xd2: padding */
    void (*state_change_fn)(void*, u32, u32); /* +0xd4: state change fn ptr */
    u16 state_halfword;         /* +0xd8: halfword state */
    u16 _padda;                 /* +0xda: unknown */
    void* hitbox_group_b;       /* +0xdc: hitbox group B ptr */
    u32 _pade0[3];              /* +0xe0: unknown */
    u32 zone_id;                /* +0xec: zone ID */
    u32 _padf0[13];             /* +0xf0: padding to 0x124 */
    void* action_state_a;       /* +0x124: action state obj A */
    void* action_state_b;       /* +0x128: action state obj B */
    void* action_state_c;       /* +0x12c: action state obj C */
    void* mover_binding;        /* +0x130: mover-controller binding obj */
} PhysicsPlayer;

/* Control flags (+0x54) */
#define PLAYER_CTRL_FACING        (1 << 0)
#define PLAYER_CTRL_BUSY          (1 << 2)   /* 0x04 */
#define PLAYER_CTRL_P             (1 << 3)   /* 0x08 */
#define PLAYER_CTRL_Q             (1 << 4)   /* 0x10 */
#define PLAYER_CTRL_PAUSABLE      (1 << 5)   /* 0x20 */
#define PLAYER_CTRL_R             (1 << 7)   /* 0x80 */

/* State flags (+0x58) */
#define PLAYER_STATE_GROUNDED     (1 << 0)   /* 0x01 */
#define PLAYER_STATE_IMPACT       (1 << 2)   /* 0x04 */
#define PLAYER_STATE_S            (1 << 4)   /* 0x10 */
#define PLAYER_STATE_T            (1 << 5)   /* 0x20 */
#define PLAYER_STATE_SPAWNING     (1 << 8)   /* 0x100 */
#define PLAYER_STATE_V            (1 << 11)  /* 0x800 */
#define PLAYER_STATE_W            (1 << 12)  /* 0x1000 */
#define PLAYER_STATE_FROZEN       (1 << 13)  /* 0x2000 */
#define PLAYER_STATE_SOLID_GUARD  (1 << 14)  /* 0x4000 */
#define PLAYER_STATE_DIR_100000   (1 << 20)  /* 0x100000 */
#define PLAYER_STATE_DIR_200000   (1 << 21)  /* 0x200000 */
#define PLAYER_STATE_X            (1 << 22)  /* 0x400000 */
#define PLAYER_STATE_Y            (1 << 26)  /* 0x4000000 */

/* Action timer (+0x62) semantics */
/* Low nibble = action phase timer (decremented 1/frame) */
/* High nibble (0xf0) = velocity-drift table index (decremented 0x10/frame) */
/* When low nibble hits 0, action_fn at +0x68 is called */
/* High nibble >> 5 (/8) indexes drift table at 0x2077944 */

/* External globals */
extern PhysicsPlayer* g_current_player;

/* Player functions */
void Player_Spawn(s32 pos_x, s32 pos_y);
void Player_Update(void* player);
void Player_Destroy(void* player);

#endif /* PLAYER_H */
