/* Input subsystem initialization functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "input.h"

/* Default repeat delay table (20 frames = 0x14) */
static const u8 default_repeat_delay[12] = {
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14
};

/* Input_SetRepeatDelay @ 0x020351d8 (48 bytes)
 * Configures auto-repeat delay for each key bit.
 * Args: r0=input_subsys, r1=key_bit (0-15), r2=delay_table ptr
 * Returns: 1 if valid key_bit, 0 otherwise */
int Input_SetRepeatDelay(InputSubsystem *input, int key_bit, u16 *delay_table) {
    int i;
    
    /* Validate key bit range (0-15) */
    if (key_bit < 0 || key_bit > 0xf) {
        return 0;
    }
    
    /* Set repeat delay for specified key bit */
    u32 *repeat_field = (u32*)((u8*)input + 0x1000);
    u32 mask = 1 << key_bit;
    
    /* Check if key is in valid range */
    if (*repeat_field & mask) {
        /* Key is valid, store delay */
        input->repeat_delay[key_bit] = delay_table[key_bit];
    }
    
    return 1;
}

/* Input_SubsysInit @ 0x02009908 (64 bytes)
 * Initializes the input subsystem.
 * Args: r0=input_subsys, r1=param1 (typically 0x52 = key config)
 * Returns: 1 on success */
int Input_SubsysInit(InputSubsystem *input, int param1) {
    /* Initialize with default repeat delays */
    int i;
    
    /* Clear input state */
    input->current_keys = 0;
    input->previous_keys = 0;
    input->pressed_edge = 0;
    input->released_edge = 0;
    input->held_keys = 0;
    
    /* Set default repeat delays (20 frames) */
    for (i = 0; i < 12; i++) {
        input->repeat_timers[i] = default_repeat_delay[i];
        input->repeat_delay[i] = default_repeat_delay[i];
        input->reload_values[i] = default_repeat_delay[i];
    }
    
    /* Clear sample area */
    for (i = 0; i < 16; i++) {
        input->samples[i] = 0;
    }
    input->contact_mask = 0;
    
    return 1;
}
