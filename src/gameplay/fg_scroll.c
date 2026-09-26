/* Gameplay foreground scroll functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"


/* FgScroll_Init @ 0x02024400 (168 bytes)
 * Initializes foreground scroll system. */
void FgScroll_Init(void) {
    g_fgscroll_count = 0;
}

/* FgScroll_Add @ 0x020244a8 (232 bytes)
 * Adds foreground scroll layer.
 * Args: r0=bg_id, r1=speed_x, r2=speed_y
 * Returns: index or -1 */
s32 FgScroll_Add(u32 bg_id, s32 speed_x, s32 speed_y) {
    if (g_fgscroll_count >= 4) return -1;
    u32 i = g_fgscroll_count;
    g_fgscrolls[i].bg_id = bg_id;
    g_fgscrolls[i].speed_x = speed_x;
    g_fgscrolls[i].speed_y = speed_y;
    g_fgscrolls[i].offset_x = 0;
    g_fgscrolls[i].offset_y = 0;
    g_fgscrolls[i].active = 1;
    g_fgscroll_count++;
    return i;
}

/* FgScroll_Remove @ 0x02024590 (296 bytes)
 * Removes foreground scroll layer.
 * Args: r0=index */
void FgScroll_Remove(u32 index) {
    if (index < g_fgscroll_count) {
        g_fgscrolls[index].active = 0;
    }
}

/* FgScroll_Update @ 0x020246b8
 * Updates foreground scroll layers. */
void FgScroll_Update(void) {
    u32 i;
    for (i = 0; i < g_fgscroll_count; i++) {
        if (g_fgscrolls[i].active) {
            g_fgscrolls[i].offset_x += g_fgscrolls[i].speed_x;
            g_fgscrolls[i].offset_y += g_fgscrolls[i].speed_y;
        }
    }
}

/* FgScroll_Draw @ 0x02024760
 * Draws foreground scroll layers. */
void FgScroll_Draw(void) {
    u32 i;
    for (i = 0; i < g_fgscroll_count; i++) {
        if (g_fgscrolls[i].active) {
            DisplayScroll_SetBgOffset(g_fgscrolls[i].bg_id,
                                      g_fgscrolls[i].offset_x >> 8,
                                      g_fgscrolls[i].offset_y >> 8);
        }
    }
}

/* FgScroll_GetOffset @ 0x020247e0
 * Returns foreground scroll offset.
 * Args: r0=index, r1=out_x, r2=out_y */
void FgScroll_GetOffset(u32 index, s32 *out_x, s32 *out_y) {
    if (index < g_fgscroll_count) {
        *out_x = g_fgscrolls[index].offset_x;
        *out_y = g_fgscrolls[index].offset_y;
    } else {
        *out_x = 0;
        *out_y = 0;
    }
}

/* FgScroll_SetSpeed @ 0x02024840
 * Sets foreground scroll speed.
 * Args: r0=index, r1=speed_x, r2=speed_y */
void FgScroll_SetSpeed(u32 index, s32 speed_x, s32 speed_y) {
    if (index < g_fgscroll_count) {
        g_fgscrolls[index].speed_x = speed_x;
        g_fgscrolls[index].speed_y = speed_y;
    }
}

/* FgScroll_IsActive @ 0x020248a0
 * Returns whether foreground scroll is active.
 * Args: r0=index
 * Returns: 1 if active, 0 otherwise */
s32 FgScroll_IsActive(u32 index) {
    if (index < g_fgscroll_count) {
        return g_fgscrolls[index].active;
    }
    return 0;
}

/* FgScroll_GetCount @ 0x020248e0
 * Returns foreground scroll count.
 * Returns: count */
u32 FgScroll_GetCount(void) {
    return g_fgscroll_count;
}

/* FgScroll_Clear @ 0x02024900
 * Clears all foreground scroll layers. */
void FgScroll_Clear(void) {
    g_fgscroll_count = 0;
}

/* FgScroll_Reset @ 0x02024920
 * Resets foreground scroll system. */
void FgScroll_Reset(void) {
    g_fgscroll_count = 0;
}
