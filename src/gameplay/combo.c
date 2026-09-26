/* Gameplay combo counter functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* Combo_Init @ 0x02021000 (148 bytes)
 * Initializes combo counter system. */
void Combo_Init(void) {
    g_combo_count = 0;
    g_combo_timer = 0;
    g_combo_max = 0;
    g_combo_active = 0;
}

/* Combo_Add @ 0x02021094 (216 bytes)
 * Adds to combo counter.
 * Args: r0=amount */
void Combo_Add(u32 amount) {
    g_combo_count += amount;
    g_combo_timer = 120;
    g_combo_active = 1;
    if (g_combo_count > g_combo_max) {
        g_combo_max = g_combo_count;
    }
}

/* Combo_Update @ 0x02021170 (264 bytes)
 * Updates combo counter. */
void Combo_Update(void) {
    if (!g_combo_active) return;
    if (g_combo_timer > 0) {
        g_combo_timer--;
    }
    if (g_combo_timer == 0) {
        g_combo_active = 0;
    }
}

/* Combo_Reset @ 0x02021278
 * Resets combo counter. */
void Combo_Reset(void) {
    g_combo_count = 0;
    g_combo_timer = 0;
    g_combo_active = 0;
}

/* Combo_Get @ 0x020212a0
 * Returns combo count.
 * Returns: count */
u32 Combo_Get(void) {
    return g_combo_count;
}

/* Combo_GetMax @ 0x020212c0
 * Returns max combo.
 * Returns: max */
u32 Combo_GetMax(void) {
    return g_combo_max;
}

/* Combo_IsActive @ 0x020212e0
 * Returns whether combo is active.
 * Returns: 1 if active, 0 otherwise */
s32 Combo_IsActive(void) {
    return g_combo_active;
}

/* Combo_GetTimer @ 0x02021300
 * Returns combo timer.
 * Returns: timer */
u32 Combo_GetTimer(void) {
    return g_combo_timer;
}

/* Combo_GetProgress @ 0x02021320
 * Returns combo timer progress.
 * Returns: progress (0-256) */
u32 Combo_GetProgress(void) {
    if (g_combo_timer == 0) return 0;
    return (g_combo_timer * 256) / 120;
}

/* Combo_GetScore @ 0x02021360
 * Returns score bonus from combo.
 * Returns: score */
u32 Combo_GetScore(void) {
    if (g_combo_count == 0) return 0;
    return g_combo_count * 100;
}

/* Combo_SetMax @ 0x020213a0
 * Sets max combo for display.
 * Args: r0=max */
void Combo_SetMax(u32 max) {
    g_combo_max = max;
}

/* Combo_AddTimer @ 0x020213c0
 * Adds time to combo timer.
 * Args: r0=amount */
void Combo_AddTimer(u32 amount) {
    g_combo_timer += amount;
}
