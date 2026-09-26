/* Input edge detection and processing.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 *
 * Reads KEYINPUT hardware register and maintains edge/released state
 * in the InputSubsystem at 0x22503e0.
 */

#include "nds_types.h"
#include "input.h"

extern InputSubsystem g_input_subsystem;

/* ======================================================================== */
/* Input_PressEdge                                                           */
/* Returns the pressed-edge mask (keys that went down this frame).          */
/* Evidence: inferred from InputSubsystem struct at 0x22503e0+0x04          */
/* ======================================================================== */
u32 Input_PressEdge(void) {
    return g_input_subsystem.pressed_edge;
}

/* ======================================================================== */
/* Input_ReleaseEdge                                                         */
/* Returns the released-edge mask (keys that went up this frame).           */
/* Evidence: inferred from InputSubsystem struct at 0x22503e0+0x06          */
/* ======================================================================== */
u32 Input_ReleaseEdge(void) {
    return g_input_subsystem.released_edge;
}

/* ======================================================================== */
/* Input_HeldRepeat                                                          */
/* Returns the held/auto-repeat mask.                                       */
/* Keys must be held past the initial delay before appearing.              */
/* Evidence: inferred from InputSubsystem struct at 0x22503e0+0x08          */
/* ======================================================================== */
u32 Input_HeldRepeat(void) {
    return g_input_subsystem.held_keys;
}

/* ======================================================================== */
/* Input_DpadRead                                                            */
/* Reads the D-pad direction from current keys.                             */
/* Returns a combined direction mask (up/down/left/right) from the          */
/* current key state, filtering to only D-pad bits.                         */
/* ======================================================================== */
u32 Input_DpadRead(void) {
    u16 keys = g_input_subsystem.current_keys;
    return keys & (KEY_DPAD_UP | KEY_DPAD_DOWN | KEY_DPAD_LEFT | KEY_DPAD_RIGHT);
}
