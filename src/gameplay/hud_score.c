#include "nds_types.h"

void HudScore_Init(void) { g_hudscore_visible = 0; g_score_display = 0; }
void HudScore_Update(void) {
    if (!g_hudscore_visible) return;
    if (g_score_display < g_game_score) { g_score_display += 100; if (g_score_display > g_game_score) g_score_display = g_game_score; }
}
void HudScore_Draw(void) { if (g_hudscore_visible) Display_PrintFixed(200, 16, "SCORE"); }
void HudScore_Show(void) { g_hudscore_visible = 1; }
void HudScore_Hide(void) { g_hudscore_visible = 0; }
s32 HudScore_IsActive(void) { return g_hudscore_visible; }
u32 HudScore_GetValue(void) { return g_score_display; }
void HudScore_Reset(void) { g_hudscore_visible = 0; g_score_display = 0; }
