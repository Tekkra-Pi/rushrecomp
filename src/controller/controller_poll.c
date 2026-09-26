/* Controller poll functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "touch_state.h"


/* Controller ring at 0x0207f02c */
extern ControllerRing g_controller_ring;

/* Controller_PollCapture @ 0x020095f8 (80 bytes)
 * Captures controller state and updates ring buffer.
 * Args: r0=ring, r1=param1, r2=counter_ptr, r3=flags_ptr
 * Returns: r4 (captured value) */
u32 Controller_PollCapture(ControllerRing *ring, u32 param1, 
                          u32 *counter_ptr, u32 *flags_ptr) {
    u32 counter;
    u32 flags;
    u32 result;
    
    /* Store param1 to ring if provided */
    if (ring) {
        ring->params[0] = param1;
    }
    
    /* Read and update counter */
    counter = *flags_ptr;
    flags = counter - 1;
    *flags_ptr = flags;
    
    /* Increment counter */
    result = *counter_ptr;
    *counter_ptr = result + 1;
    
    /* Call poll function */
    Controller_Poll();
    
    return param1;
}

/* Controller_PollCapture2 @ 0x02009650 (80 bytes)
 * Second variant of controller poll capture.
 * Args: r0=ring, r1=param1
 * Similar to PollCapture but with different register usage */
void Controller_PollCapture2(ControllerRing *ring, u32 param1) {
    if (ring) {
        ring->params[0] = param1;
    }
    
    Controller_Poll();
}

/* Controller_Unknown85c @ 0x0200985c (32 bytes)
 * Unknown controller function - initializes ring state.
 * Args: r0=ring */
void Controller_Unknown85c(ControllerRing *ring) {
    ring->params[4] = 0;  /* Halfword at +0x0a */
    ring->callback = 0;
    ring->table = 0;
    ring->count = 0;
    ring->mode = 0;
    
    /* Clear DMA params area */
    memset(&ring->dma_params, 0, sizeof(ring->dma_params));
}
