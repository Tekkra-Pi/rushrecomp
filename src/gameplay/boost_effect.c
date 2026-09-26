/* Gameplay boost effect functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_boostfx_active;
static u32 s_boostfx_timer;

void BoostEffect_Init(void) {
    s_boostfx_active = 0;
    s_boostfx_timer = 0;
    g_boostfx_intensity = 0;
}

void BoostEffect_Start(void) {
    s_boostfx_active = 1;
    s_boostfx_timer = 0;
    g_boostfx_intensity = 1;
}

void BoostEffect_Update(void) {
    if (!s_boostfx_active) return;
    s_boostfx_timer++;
}

void BoostEffect_Stop(void) {
    s_boostfx_active = 0;
    g_boostfx_intensity = 0;
}

s32 BoostEffect_IsActive(u32 index) {
    (void)index;
    return s_boostfx_active;
}

u32 BoostEffect_GetTimer(void) { return s_boostfx_timer; }

u32 BoostEffect_GetIntensity(void) { return g_boostfx_intensity; }

void BoostEffect_Draw(void) {
}

void BoostEffect_SetIntensity(u32 index, u32 value) {
    (void)index;
    g_boostfx_intensity = value;
}

void BoostEffect_Reset(void) {
    s_boostfx_active = 0;
    s_boostfx_timer = 0;
    g_boostfx_intensity = 0;
}
