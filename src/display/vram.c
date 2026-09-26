/* VRAM bank control functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* VRAM bank state at 0x02089234 */
extern u32 g_vram_state;

/* VRAMBank_Configure @ 0x02004c94 (176 bytes)
 * Configures a VRAM bank.
 * Args: r0=bank_id, r1=config */
void VRAMBank_Configure(u32 bank_id, u32 config) {
    u8 *cr = (u8*)&VRAM_A_CR + bank_id;
    
    /* Set bank configuration */
    *cr = config & 0xff;
}

/* VRAMBank_Enable @ 0x02004d44 (220 bytes)
 * Enables a VRAM bank.
 * Args: r0=bank_id */
void VRAMBank_Enable(u32 bank_id) {
    u8 *cr = (u8*)&VRAM_A_CR + bank_id;
    
    /* Enable bank */
    *cr |= 0x80;
}

/* VRAMBank_Disable @ 0x02004e20 (160 bytes)
 * Disables a VRAM bank.
 * Args: r0=bank_id */
void VRAMBank_Disable(u32 bank_id) {
    u8 *cr = (u8*)&VRAM_A_CR + bank_id;
    
    /* Disable bank */
    *cr &= ~0x80;
}
