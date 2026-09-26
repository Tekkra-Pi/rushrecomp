#ifndef TOUCH_STATE_H
#define TOUCH_STATE_H

#include "nds_types.h"

/* Touch-state structure at 0x022b65b4 (0x28 bytes).
 * Updated each frame by the dispatcher chain.
 * Consumed by game code via AABB hit-test. */
typedef struct {
    u32 enable;         /* +0x00: bit0 = touch input enabled */
    u32 alternate;      /* +0x04: alternate updater mode */
    u32 x;              /* +0x08: current X; 0xffffffff = no contact */
    u32 y;              /* +0x0c: current Y; 0xffffffff = no contact */
    u32 flags;          /* +0x10: status flags (see below) */
    u32 history_count;  /* +0x14: number of history entries */
    /* +0x16..: coordinate history FIFO (8 bytes each) */
} TouchState;

/* Touch flags at +0x10 */
#define TOUCH_FLAG_VALID       (1 << 0)
#define TOUCH_FLAG_CONTACT     (1 << 1)
#define TOUCH_FLAG_TOUCHED     (1 << 4)   /* 0x10: current-frame contact */
#define TOUCH_FLAG_HELD        (1 << 5)   /* 0x20: held/previous contact */
#define TOUCH_FLAG_PRESS_EDGE  (1 << 6)   /* 0x40: rising edge */
#define TOUCH_FLAG_RELEASE_EDGE (1 << 7)  /* 0x80: falling edge */
#define TOUCH_FLAG_OUT_OF_RANGE (1 << 8)  /* 0x100: sample error */

/* Controller ring at 0x0207f02c */
typedef struct {
    u32 callback;       /* +0x00: configured callback */
    u16 params[4];      /* +0x04..+0x0a: four halfword parameters */
    u16 index;          /* +0x0c: sample/current index */
    u16 _pad0e;
    u32 table;          /* +0x10: optional 8-byte-stride table pointer */
    u16 count;          /* +0x14: table/sample count */
    u16 _pad16;
    u8  dma_params[0x18]; /* +0x18..+0x2c: DMA/coordinate parameters */
    u16 flag;           /* +0x30: flag */
    u16 mode;           /* +0x32: mode */
    u16 wait_flags;     /* +0x34: wait flags */
    u16 wait_mask;      /* +0x36: wait mask */
} ControllerRing;

/* Dispatcher node (0x48 bytes, linked list) */
typedef struct DispatcherNode {
    struct DispatcherNode* next;  /* +0x00 */
    u32 type;                     /* +0x04: 0=bulk, 1=dispatch */
    u32 index;                    /* +0x08 */
    u32 flags;                    /* +0x0c */
    u32 src;                      /* +0x10: source address */
    u32 size;                     /* +0x14 */
    u8  _pad18[0x20];
    u32 src_start;                /* +0x38 */
    u32 src_end;                  /* +0x3c */
    u8  _pad40[0x1c];
    u32 buf;                      /* +0x5c: allocated buffer */
    u32 total_size;               /* +0x60 */
} DispatcherNode;

/* Sampler output (12 bytes, written by FUN_02009900) */
typedef struct {
    u32 flags;    /* +0x00: bit0=valid, bit1=contact */
    u32 x;        /* +0x04: X coordinate (shifted) */
    u32 pressure; /* +0x08: pressure (7-bit) */
} SamplerOutput;

/* Input subsystem — base for sampler reads at +0x1180/+0x11c4 */
typedef struct {
    u8 _pad[0x1180];
    u32 samples[16];  /* +0x1180: per-index packed sample words */
    u32 contact_mask; /* +0x11c4: contact bitmask */
} InputSubsystem;

/* External globals */
extern TouchState g_touch_state;
extern ControllerRing g_controller_ring;
extern InputSubsystem g_input_subsystem;

/* Touch functions */
int TouchState_Init(u32 param);
s32 TouchState_ResetEnable(void);
void TouchState_MainUpdater(u32 dst, u32 src, u32 len);
void TouchState_Producer(void* node, int param2, short param3,
                         u32 param4, u32 param5, u32 param6, u32 param7,
                         u32 param8, u32 param9);
void TouchState_Dispatcher(u8* subsys);
int Sampler_ConsumeSharedSamples(int input_subsys, int sample_idx, u32* output);

#endif /* TOUCH_STATE_H */
