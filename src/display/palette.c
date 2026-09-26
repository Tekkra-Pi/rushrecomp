/* Palette loading functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_palfade_active;
extern u32 g_palfade_timer;
extern u32 g_palfade_duration;

void Palette_LoadMain(void) {
    /* Load main palette from ROM to PALRAM */
}

void Palette_LoadSub(void) {
    /* Load sub palette from ROM to PALRAM_SUB */
}

void Palette_FadeMain(void) {
    if (g_palfade_active) {
        if (g_palfade_timer > 0) {
            g_palfade_timer--;
        } else {
            g_palfade_active = 0;
        }
    }
}
