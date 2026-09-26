/* System timer functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* SysTimer_Init @ 0x0200bc00 (148 bytes)
 * Initializes system timer. */
void SysTimer_Init(void) {
    /* Clear timer state */
    g_timer_ticks = 0;
    g_timer_overflow = 0;
    
    /* Configure timer */
    REG_TM0CNT = 0;
    REG_TM0D = 0;
    REG_TM0CNT = 0xC1;  /* Enable, prescaler /64, IRQ on overflow */
}

/* SysTimer_GetTicks @ 0x0200bc94 (216 bytes)
 * Returns current timer ticks.
 * Returns: tick count */
u32 SysTimer_GetTicks(void) {
    return g_timer_ticks;
}

/* SysTimer_GetMs @ 0x0200bd70 (264 bytes)
 * Returns current time in milliseconds.
 * Returns: milliseconds */
u32 SysTimer_GetMs(void) {
    return g_timer_ticks / 64;
}

/* SysTimer_GetSeconds @ 0x0200be78
 * Returns current time in seconds.
 * Returns: seconds */
u32 SysTimer_GetSeconds(void) {
    return g_timer_ticks / (64 * 60);
}

/* SysTimer_Delay @ 0x0200bea0
 * Delays for specified ticks.
 * Args: r0=ticks */
void SysTimer_Delay(u32 ticks) {
    u32 start = g_timer_ticks;
    while ((g_timer_ticks - start) < ticks);
}

/* SysTimer_DelayMs @ 0x0200bee0
 * Delays for specified milliseconds.
 * Args: r0=ms */
void SysTimer_DelayMs(u32 ms) {
    SysTimer_Delay(ms * 64);
}

/* SysTimer_DelaySeconds @ 0x0200bf20
 * Delays for specified seconds.
 * Args: r0=seconds */
void SysTimer_DelaySeconds(u32 seconds) {
    SysTimer_Delay(seconds * 64 * 60);
}

/* SysTimer_Update @ 0x0200bf60
 * Updates timer (called from VBlank ISR). */
void SysTimer_Update(void) {
    g_timer_ticks++;
}
