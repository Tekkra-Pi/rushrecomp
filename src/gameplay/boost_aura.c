/* Gameplay boost aura functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

void BoostAura_Init(void) {
    g_boostaura_alpha = 0;
    g_boostaura_size = 0;
    g_boostfx_intensity = 0;
}

void BoostAura_Start(void) {
    g_boostaura_alpha = 16;
    g_boostaura_size = 0x400;
    g_boostfx_intensity = 1;
}

void BoostAura_Update(void) {
    if (g_boostlaunch_active) {
        if (g_boostaura_alpha < 255) g_boostaura_alpha += 4;
        if (g_boostaura_size < 0x800) g_boostaura_size += 0x20;
    } else {
        if (g_boostaura_alpha > 0) g_boostaura_alpha -= 8;
        if (g_boostaura_size > 0) g_boostaura_size -= 0x40;
    }
}

void BoostAura_Draw(void) {
}

void BoostAura_Stop(void) {
    g_boostaura_alpha = 0;
    g_boostaura_size = 0;
    g_boostfx_intensity = 0;
}

s32 BoostAura_IsActive(u32 index) {
    (void)index;
    return g_boostaura_alpha > 0;
}

u32 BoostAura_GetTimer(void) { return 0; }

s32 BoostAura_GetSize(u32 index) {
    (void)index;
    return g_boostaura_size;
}

s32 BoostAura_GetAlpha(u32 index) {
    (void)index;
    return g_boostaura_alpha;
}

void BoostAura_Reset(void) {
    g_boostaura_alpha = 0;
    g_boostaura_size = 0;
    g_boostfx_intensity = 0;
}
