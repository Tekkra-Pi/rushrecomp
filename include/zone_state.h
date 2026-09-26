#ifndef ZONE_STATE_H
#define ZONE_STATE_H

#include "nds_types.h"

/* Zone state struct at 0x22c4560
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 725-737 */
typedef struct {
    u16 stage_id;               /* +0x00: stage ID (half) */
    u16 _pad02;                 /* +0x02: padding */
    u32 _pad04[5];              /* +0x04: unknown */
    u16 mode_word_10;           /* +0x10: mode word (0=title, 2=demo, 3=gameplay) */
    u16 _pad12;                 /* +0x12: padding */
    u32 flags_14;               /* +0x14: flags (bit2/4/8 cleared during transitions) */
    u8  _pad18;                 /* +0x18: unknown */
    u8  flags_19;               /* +0x19: flags (bit0x40 cleared by mode-entity) */
    u16 _pad1a;                 /* +0x1a: padding */
    u8  zone_byte_1c;           /* +0x1c: zone byte from stage table */
    u8  zone_byte_1d;           /* +0x1d: zone byte from stage table */
    u16 _pad1e;                 /* +0x1e: padding */
    u32 key_checksum;           /* +0x20: key checksum */
    u32 flags_24;               /* +0x24: set on valid key */
} ZoneState;

/* Game-state context at 0x22c4490
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 708-709 */
typedef struct {
    u32 _pad00;                 /* +0x00: unknown */
    u32 mode_ctx;               /* +0x04: mode context */
    u32 _pad08[17];             /* +0x08: unknown */
    u32 stage_id_48;            /* +0x48: stage ID */
    u8  stage_byte_4c;          /* +0x4c: stage byte from zone-state */
    u8  stage_byte_4d;          /* +0x4d: stage byte from zone-state */
    u16 _pad4e;                 /* +0x4e: padding */
} GameStateContext;

/* Mode-transition entity data at 0x22c4410
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 707, 711-729 */
typedef struct {
    u32 flags_00;               /* +0x00: flags (bit0x40 triggers dispatch) */
    u32 counter_02;             /* +0x02: counter (decremented toward target) */
    u32 mode_04;                /* +0x04: mode argument (0x43='C', 0x42='B') */
    u16 target_06;              /* +0x06: target (0x100 or 0x5000000) */
    u16 _pad08;                 /* +0x08: padding */
    u32 _pad0c[2];              /* +0x0c: unknown */
    u32 data_10;                /* +0x10: derived value (0x10 if counter>=0x1000 else counter>>8) */
} ModeTransitionData;

/* Stage object-loader table at 0x2074184
 * Evidence: CONFIRMED-STATIC from HANDOFF.md line 496-498 */
/* 120 entries, 10 zone rows, 12 act slots per row */
/* Rows 0-5: main zones; rows 6-9: special/boss resources */

/* Camera bounds struct at 0x22c4dcc
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 97 */
typedef struct {
    s32 left;                   /* +0x00: left bound */
    s32 top;                    /* +0x04: top bound */
    s32 right;                  /* +0x08: right bound */
    s32 bottom;                 /* +0x0c: bottom bound */
} CameraBounds;

/* Score-related state at 0x22c4478
 * Evidence: CONFIRMED-STATIC from HANDOFF.md */
typedef struct {
    u8 _pad00[0x10];            /* +0x00: unknown */
    s8 signed_byte_10;          /* +0x10: signed byte + 6 */
    u8 _pad11;                  /* +0x11: padding */
    u8 _pad12[2];               /* +0x12: unknown */
    u16 halfword_14;            /* +0x14: halfword (0xfff typical) */
    u16 halfword_16;            /* +0x16: halfword (0 typical) */
    u8 _pad18[4];               /* +0x18: unknown */
    u32 base;                   /* +0x1c: base value (0xb50 typical) */
} ScoreState;

/* Collision entity list at 0x22c4e64
 * Evidence: CONFIRMED-STATIC from HANDOFF.md line 412-413 */
typedef struct {
    u32 count;                  /* +0x00: collision entity count */
    void* list;                 /* +0x04: collision entity list pointer */
} CollisionEntityList;

/* Action data root at 0x22c45f0
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 701 */
typedef struct {
    u32 _pad00;                 /* +0x00: unknown (only ref = 0204dbb0) */
} ActionDataRoot;

/* Object cache root at 0x22c448c
 * Evidence: CONFIRMED-STATIC from HANDOFF.md */
typedef struct {
    void* ptr;                  /* +0x00: pointer to cached objects */
} ObjectCacheRoot;

/* External globals */
extern ZoneState g_zone_state;
extern GameStateContext g_game_state;
extern ModeTransitionData *g_mode_transition;
extern CameraBounds g_camera_bounds;
extern CollisionEntityList g_collision_list;

/* Zone/stage functions */
void Zone_Init(u32 stage_id);
void Mode_SetTransition(u32 mode, u32 data);

#endif /* ZONE_STATE_H */
