#ifndef INPUT_H
#define INPUT_H

#include "nds_types.h"

/* Input subsystem struct at 0x22503e0
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 176-186 */
typedef struct {
    u16 current_keys;           /* +0x00: current keys this frame (polled) */
    u16 previous_keys;          /* +0x02: previous keys last frame */
    u16 pressed_edge;           /* +0x04: pressed edge (current & ~prev) */
    u16 released_edge;          /* +0x06: released edge (~current & prev) */
    u16 held_keys;              /* +0x08: held keys past auto-repeat delay */
    u8  repeat_timers[12];      /* +0x0a: running repeat timers (12 bits) */
    u8  _pad16[0xa];           /* +0x16: padding */
    u8  repeat_delay[12];       /* +0x22: repeat delay config */
    u8  _pad2e[0xc];           /* +0x2e: padding */
    u8  reload_values[12];      /* +0x3a: reload values */
    u8  _pad46[0x1a];          /* +0x46: padding */
    /* +0x1180: per-index packed sample words (for touch sampler) */
    u32 samples[16];            /* +0x1180: packed sample words */
    u32 contact_mask;           /* +0x11c4: contact bitmask */
} InputSubsystem;

/* Demo script controller data (0x20 bytes)
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 193-203 */
typedef struct {
    u16 flags;                  /* +0x00: bit0=reset on next tick, bit1=script active */
    u16 _pad02;                 /* +0x02: padding */
    u32 total_frames;           /* +0x04: total frames elapsed */
    u32 current_step;           /* +0x08: current script-step index */
    u16 frames_in_step;         /* +0x0c: frames elapsed in current step */
    u16 last_global_mode;       /* +0x0e: last global sequence mode */
    u16 transition_countdown;   /* +0x10: 8-frame transition countdown */
    u16 _pad12;                 /* +0x12: padding */
    u32 spawner_arg;            /* +0x14: spawner argument / sequence selector */
    void* script_entries;       /* +0x18: pointer to script entries (4-byte stride) */
    void* resource_handle;      /* +0x1c: loaded script/resource handle */
} DemoScriptController;

/* NDS hardware registers */
#define KEYINPUT_REG   0x4000130
#define KEYCNT_REG     0x4000136

/* Button masks */
#define KEY_A          (1 << 0)
#define KEY_B          (1 << 1)
#define KEY_SELECT     (1 << 2)
#define KEY_START      (1 << 3)
#define KEY_DPAD_RIGHT (1 << 4)
#define KEY_DPAD_LEFT  (1 << 5)
#define KEY_DPAD_UP    (1 << 6)
#define KEY_DPAD_DOWN  (1 << 7)
#define KEY_R          (1 << 8)
#define KEY_L          (1 << 9)

/* External globals */
extern InputSubsystem g_input_subsystem;
extern DemoScriptController g_demo_controller;

/* Input functions */
void Input_Init(void);
void Input_Poll(void);
void Input_DebounceEdgeRead(InputSubsystem *input, u16 new_keys);
int Input_SetRepeatDelay(InputSubsystem *input, int key_bit, u16 *delay_table);

#endif /* INPUT_H */
