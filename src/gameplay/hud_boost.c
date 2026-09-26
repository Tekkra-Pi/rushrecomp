#include "nds_types.h"

void HudBoost_Init(void) { g_hudboost_visible = 0; g_hud_boost = 0; }
void HudBoost_Update(void) { if (g_hudboost_visible) g_hud_boost = g_boost_level; }
void HudBoost_Draw(void) { if (g_hudboost_visible) Display_PrintFixed(200, 16, "BOOST"); }
void HudBoost_Show(void) { g_hudboost_visible = 1; }
void HudBoost_Hide(void) { g_hudboost_visible = 0; }
s32 HudBoost_IsActive(void) { return g_hudboost_visible; }
u32 HudBoost_GetValue(void) { return g_hud_boost; }
void HudBoost_Reset(void) { g_hudboost_visible = 0; g_hud_boost = 0; }
