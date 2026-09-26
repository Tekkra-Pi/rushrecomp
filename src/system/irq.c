/* IME/IRQ management functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* IRQ handler table at 0x02089230 */
extern void (*g_irq_handlers[16])(void);

/* IME_Enable @ 0x0207f000 (148 bytes)
 * Enables interrupt master enable. */
void IME_Enable(void) {
    REG_IME = 1;
}

/* IME_Disable @ 0x0207f094 (216 bytes)
 * Disables interrupt master enable. */
void IME_Disable(void) {
    REG_IME = 0;
}

/* IRQ_SetHandler @ 0x0207f16c (284 bytes)
 * Sets IRQ handler for specified interrupt.
 * Args: r0=irq_bit, r1=handler */
void IRQ_SetHandler(u32 irq_bit, void (*handler)(void)) {
    if (irq_bit < 16) {
        g_irq_handlers[irq_bit] = handler;
    }
}

/* IRQ_Enable @ 0x0207f28c
 * Enables specified interrupt.
 * Args: r0=irq_bit */
void IRQ_Enable(u32 irq_bit) {
    if (irq_bit < 16) {
        REG_IE |= (1 << irq_bit);
    }
}

/* IRQ_Disable @ 0x0207f2c0
 * Disables specified interrupt.
 * Args: r0=irq_bit */
void IRQ_Disable(u32 irq_bit) {
    if (irq_bit < 16) {
        REG_IE &= ~(1 << irq_bit);
    }
}

/* IRQ_Handler @ 0x0207f300
 * Main IRQ handler - dispatches to specific handlers. */
void IRQ_Handler(void) {
    u32 irq = REG_IF;
    u32 i;
    
    /* Handle each pending interrupt */
    for (i = 0; i < 16; i++) {
        if (irq & (1 << i)) {
            if (g_irq_handlers[i]) {
                g_irq_handlers[i]();
            }
        }
    }
    
    /* Acknowledge interrupts */
    REG_IF = irq;
}
