/* System interrupt functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* Interrupt_Init @ 0x0200b800 (168 bytes)
 * Initializes interrupt system. */
void Interrupt_Init(void) {
    /* Clear interrupt state */
    u32 i;
    for (i = 0; i < 16; i++) {
        g_interrupt_handlers[i].active = 0;
        g_interrupt_handlers[i].func = NULL;
    }
    
    /* Enable interrupts */
    REG_IME = 1;
}

/* Interrupt_Register @ 0x0200b8a8 (232 bytes)
 * Registers interrupt handler.
 * Args: r0=interrupt_id, r1=handler
 * Returns: 1 if success, 0 if full */
s32 Interrupt_Register(u32 interrupt_id, void (*handler)(void)) {
    u32 i;
    
    if (interrupt_id >= 16) return 0;
    
    /* Find free slot */
    for (i = 0; i < 16; i++) {
        if (!g_interrupt_handlers[i].active) {
            g_interrupt_handlers[i].active = 1;
            g_interrupt_handlers[i].id = interrupt_id;
            g_interrupt_handlers[i].func = handler;
            
            /* Enable interrupt */
            REG_IE |= (1 << interrupt_id);
            
            return 1;
        }
    }
    
    return 0;
}

/* Interrupt_Unregister @ 0x0200b990 (296 bytes)
 * Unregisters interrupt handler.
 * Args: r0=interrupt_id */
void Interrupt_Unregister(u32 interrupt_id) {
    u32 i;
    
    for (i = 0; i < 16; i++) {
        if (g_interrupt_handlers[i].active && g_interrupt_handlers[i].id == interrupt_id) {
            g_interrupt_handlers[i].active = 0;
            
            /* Disable interrupt */
            REG_IE &= ~(1 << interrupt_id);
            
            return;
        }
    }
}

/* Interrupt_Enable @ 0x0200bab8
 * Enables specific interrupt.
 * Args: r0=interrupt_id */
void Interrupt_Enable(u32 interrupt_id) {
    REG_IE |= (1 << interrupt_id);
}

/* Interrupt_Disable @ 0x0200bae0
 * Disables specific interrupt.
 * Args: r0=interrupt_id */
void Interrupt_Disable(u32 interrupt_id) {
    REG_IE &= ~(1 << interrupt_id);
}

/* Interrupt_Handler @ 0x0200bb00
 * Main interrupt handler (called from ISR). */
void Interrupt_Handler(void) {
    u32 i;
    u32 iflags = REG_IF;
    
    for (i = 0; i < 16; i++) {
        if (iflags & (1 << i) && g_interrupt_handlers[i].active) {
            if (g_interrupt_handlers[i].func) {
                g_interrupt_handlers[i].func();
            }
        }
    }
    
    /* Acknowledge interrupts */
    REG_IF = iflags;
}

/* Interrupt_WaitVBlank @ 0x0200bbc0
 * Waits for VBlank interrupt. */
void Interrupt_WaitVBlank(void) {
    REG_IF = IRQ_VBLANK;
    while (!(REG_IF & IRQ_VBLANK));
}
