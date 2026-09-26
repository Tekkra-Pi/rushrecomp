#include "nds_types.h"

void HudCombo_Init(void) { g_hudcombo_visible = 0; g_hud_combo = 0; }
void HudCombo_Update(void) { if (g_hudcombo_visible) g_hud_combo = g_combo_counter; }
void HudCombo_Draw(void) { if (g_hudcombo_visible) Display_PrintFixed(180, 40, "COMBO"); }
void HudCombo_Show(void) { g_hudcombo_visible = 1; }
void HudCombo_Hide(void) { g_hudcombo_visible = 0; }
s32 HudCombo_IsActive(void) { return g_hudcombo_visible; }
u32 HudCombo_GetValue(void) { return g_hud_combo; }
void HudCombo_Reset(void) { g_hudcombo_visible = 0; g_hud_combo = 0; }
