/* Controller ring buffer functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md and HANDOFF.md
 *
 * controller_check_flag @ 0x02009160 (disassembled from ROM)
 *   Checks if a flag bit is set in the controller ring struct.
 *   Compares current sample index against a threshold loaded from +0x34.
 *   Returns 1 if flag condition met, 0 otherwise.
 *
 * controller_wait_flag @ 0x0200918c (same block, inverted logic)
 *   Waits/checks the complementary flag condition.
 *
 * controller_copy_sample_slot @ 0x020091ac (48 bytes)
 *   Copies one 8-byte sample slot from the controller ring buffer.
 *   Checks +0x30 flag before copying.
 */

#include "nds_types.h"

/* Controller ring struct at 0x0207f02c */
typedef struct {
    u32 callback;           /* +0x00: configured callback */
    u16 param_a;            /* +0x04: parameter */
    u16 param_b;            /* +0x06: parameter */
    u16 param_c;            /* +0x08: parameter */
    u16 param_d;            /* +0x0a: parameter */
    u32 sample_index;       /* +0x0c: current sample index */
    void* table;            /* +0x10: optional 8-byte stride table */
    u32 table_count;        /* +0x14: table/sample count */
    u32 _pad18[6];          /* +0x18: DMA/coordinate parameters */
    u16 flag;               /* +0x30: flag */
    u16 mode;               /* +0x32: mode */
    u16 wait_flags;         /* +0x34: wait flags */
    u16 wait_mask;          /* +0x36: wait mask */
} ControllerRing;

extern ControllerRing g_controller_ring;
extern u32 g_controller_index;

/* External sync functions */
extern void Controller_Lock(void);
extern void Controller_Unlock(void);

/* ======================================================================== */
/* controller_check_flag @ 0x02009160                                       */
/* Checks if a flag bit is set in the controller ring.                      */
/* Reads the ring's wait_flags (+0x34) and compares against current index.  */
/* Returns: 1 if flag condition met, 0 otherwise                           */
/* ======================================================================== */
int Controller_CheckFlag(void) {
    u32 index;
    u32 threshold;
    u32 diff;
    int result;

    Controller_Lock();

    index = g_controller_ring.sample_index;
    threshold = g_controller_ring.wait_flags;

    if (index > threshold) {
        diff = index - threshold;
        /* Check if difference indicates flag is set (overflow check) */
        result = (diff >= 0x80000000u) ? 1 : 0;
    } else {
        diff = threshold - index;
        /* Inverted check for wait path */
        result = (diff >= 0x80000000u) ? 0 : 1;
    }

    Controller_Unlock();
    return result;
}

/* ======================================================================== */
/* controller_wait_flag @ 0x0200918c                                        */
/* Waits/checks the complementary flag condition.                           */
/* Same structure as check_flag but with inverted result logic.             */
/* Returns: 1 if ready (flag cleared), 0 if still waiting                  */
/* ======================================================================== */
int Controller_WaitFlag(void) {
    u32 index;
    u32 threshold;
    u32 diff;
    int result;

    Controller_Lock();

    index = g_controller_ring.sample_index;
    threshold = g_controller_ring.wait_flags;

    if (index > threshold) {
        diff = index - threshold;
        result = (diff >= 0x80000000u) ? 0 : 1;
    } else {
        diff = threshold - index;
        result = (diff >= 0x80000000u) ? 1 : 0;
    }

    Controller_Unlock();
    return result;
}

/* ======================================================================== */
/* controller_copy_sample_slot @ 0x020091ac                                */
/* Copies one 8-byte sample slot from the controller ring buffer.           */
/* Checks +0x30 flag before copying.                                        */
/* Args: r0=ring struct pointer                                              */
/* ======================================================================== */
void Controller_CopySampleSlot(void) {
    Controller_Lock();

    {
        ControllerRing *ring = &g_controller_ring;
        u32 flag_val = ring->flag;

        if (flag_val != 0) {
            u32 *src;
            u32 *dst;

            /* Load from ring's current position */
            src = (u32 *)((u8 *)ring + ring->sample_index * 8);

            /* Copy to destination based on flag state */
            if (ring->wait_flags == 0) {
                dst = (u32 *)((u8 *)ring + 0x18);
            } else {
                dst = (u32 *)((u8 *)ring + 0x1c);
            }

            dst[0] = src[0];
            dst[1] = src[1];
        }
    }

    Controller_Unlock();
}
