#include "nds_types.h"
void WaterOverlay_Init(void) { g_wateroverlay_active = 0; }
void WaterOverlay_Start(void) { g_wateroverlay_active = 1; }
void WaterOverlay_Stop(void) { g_wateroverlay_active = 0; }
void WaterOverlay_Update(void) { }
void WaterOverlay_Draw(void) { }
s32 WaterOverlay_IsActive(u32 index) { (void)index; return g_wateroverlay_active; }
void WaterOverlay_Reset(void) { g_wateroverlay_active = 0; }
