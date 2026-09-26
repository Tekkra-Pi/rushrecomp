/* Display priority and layer functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* DisplayPriority_Init @ 0x02006600 (168 bytes)
 * Initializes display priority system. */
void DisplayPriority_Init(void) {
    /* Clear priority lists */
    /* ... */
}

/* DisplayPriority_Set @ 0x020066a8 (232 bytes)
 * Sets display priority for entity.
 * Args: r0=entity, r1=priority */
void DisplayPriority_Set(void *entity, u32 priority) {
    s32 *data = (s32*)entity;
    data[17] = priority;  /* +0x44: display priority */
}

/* DisplayPriority_Get @ 0x02006790 (296 bytes)
 * Returns display priority for entity.
 * Args: r0=entity
 * Returns: display priority */
u32 DisplayPriority_Get(void *entity) {
    s32 *data = (s32*)entity;
    return data[17];
}

/* DisplayPriority_Sort @ 0x020068b8
 * Sorts entities by display priority. */
void DisplayPriority_Sort(void) {
    /* Sort entity list by priority */
    /* ... */
}

/* DisplayLayer_Set @ 0x02006900
 * Sets display layer for entity.
 * Args: r0=entity, r1=layer */
void DisplayLayer_Set(void *entity, u32 layer) {
    s32 *data = (s32*)entity;
    data[18] = layer;  /* +0x48: display layer */
}

/* DisplayLayer_Get @ 0x02006930
 * Returns display layer for entity.
 * Args: r0=entity
 * Returns: display layer */
u32 DisplayLayer_Get(void *entity) {
    s32 *data = (s32*)entity;
    return data[18];
}

/* DisplayLayer_IsVisible @ 0x02006960
 * Returns whether entity is visible.
 * Args: r0=entity
 * Returns: 1 if visible, 0 otherwise */
s32 DisplayLayer_IsVisible(void *entity) {
    u32 layer = DisplayLayer_Get(entity);
    
    /* Check if layer is enabled */
    return (g_display_layer_mask & (1 << layer)) ? 1 : 0;
}

/* DisplayLayer_SetMask @ 0x020069a0
 * Sets display layer visibility mask.
 * Args: r0=mask */
void DisplayLayer_SetMask(u32 mask) {
    g_display_layer_mask = mask;
}
