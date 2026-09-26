#include "nds_types.h"
void SpeedEffect_Init(void) { g_speedeffect_active = 0; g_speedeffect_timer = 0; }
void SpeedEffect_Start(void) { g_speedeffect_active = 1; g_speedeffect_timer = 0; }
void SpeedEffect_Stop(void) { g_speedeffect_active = 0; }
void SpeedEffect_Update(void) {
    if (!g_speedeffect_active) return;
    g_speedeffect_timer++;
}
s32 SpeedEffect_IsActive(u32 index) { (void)index; return g_speedeffect_active; }
void SpeedEffect_Reset(void) { g_speedeffect_active = 0; g_speedeffect_timer = 0; }
