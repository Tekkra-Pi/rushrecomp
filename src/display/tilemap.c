/* Tilemap and background functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_stage_tileset_id;
extern u32 g_tileanim_count;
extern AnimEntry g_tileanims[];

void Tilemap_Load(void) {
    /* Load tilemap data for current stage */
    g_tileanim_count = 0;
}

void Tilemap_Load8x8(void) {
    /* Load 8x8 tile mode background */
}

void Tilemap_Update(void) {
    u32 i;
    for (i = 0; i < g_tileanim_count; i++) {
        if (g_tileanims[i].active) {
            g_tileanims[i].timer++;
            if (g_tileanims[i].timer >= g_tileanims[i].speed) {
                g_tileanims[i].timer = 0;
                g_tileanims[i].frame++;
                if (g_tileanims[i].frame >= g_tileanims[i].num_frames) {
                    g_tileanims[i].frame = 0;
                }
            }
        }
    }
}

void Tilemap_SetPalette(u32 index, u32 value) {
    (void)index;
    (void)value;
}
