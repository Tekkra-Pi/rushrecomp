/* Input debounce/edge detection @ 0x02035134 (164 bytes)
 * Performs pressed/released edge detection and auto-repeat for 12 key bits.
 * Args: r0=InputSubsystem*, r1=new_keys (u16)
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 175-188 */

#include "input.h"

void Input_DebounceEdgeRead(InputSubsystem *input, u16 new_keys) {
    u32 i;
    
    /* Save current to previous, store new keys */
    input->previous_keys = input->current_keys;
    input->current_keys = new_keys;
    
    /* Edge detection: pressed = current & ~previous */
    input->pressed_edge = input->current_keys & 
                          (input->current_keys ^ input->previous_keys);
    
    /* Edge detection: released = previous & ~current */
    input->released_edge = input->previous_keys & 
                           (input->current_keys ^ input->previous_keys);
    
    /* Initialize held = pressed (keys just pressed this frame) */
    input->held_keys = input->pressed_edge;
    
    /* Auto-repeat loop for 12 key bits */
    for (i = 0; i < 12; i++) {
        u16 mask = 1 << i;
        
        if (!(input->current_keys & mask)) {
            /* Key not pressed: reload timer from config */
            input->repeat_timers[i] = input->repeat_delay[i];
        } else {
            /* Key pressed: decrement timer or set held bit */
            if (input->repeat_timers[i] > 0) {
                input->repeat_timers[i]--;
            } else {
                /* Timer expired: key is held, reload timer */
                input->held_keys |= mask;
                input->repeat_timers[i] = input->reload_values[i];
            }
        }
    }
}
