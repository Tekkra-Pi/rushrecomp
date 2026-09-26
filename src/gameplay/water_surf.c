/* Gameplay level water surface functions. */
#include "nds_types.h"


void WaterSurf_Init(void) { g_watersurf_active = 0; g_watersurf_y = 0; g_watersurf_amp = 0; g_watersurf_speed = 0; }

void WaterSurf_Start(s32 y, s32 amp, s32 speed) {
    g_watersurf_active = 1; g_watersurf_y = y;
    g_watersurf_amp = amp; g_watersurf_speed = speed;
}

void WaterSurf_Stop(void) { g_watersurf_active = 0; }

s32 WaterSurf_GetY(s32 x) {
    if (!g_watersurf_active) return 0x7fffffff;
    return g_watersurf_y + (Math_Sin(x * g_watersurf_speed / 256) * g_watersurf_amp / 256);
}

s32 WaterSurf_IsActive(void) { return g_watersurf_active; }
s32 WaterSurf_GetBaseY(void) { return g_watersurf_y; }
s32 WaterSurf_GetAmp(void) { return g_watersurf_amp; }
s32 WaterSurf_GetSpeed(void) { return g_watersurf_speed; }
void WaterSurf_Reset(void) { g_watersurf_active = 0; g_watersurf_y = 0; g_watersurf_amp = 0; g_watersurf_speed = 0; }
