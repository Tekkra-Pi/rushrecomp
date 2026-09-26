#include "nds_types.h"
void HUD_Init(void) { g_hudscore_visible = 0; g_hudrings_visible = 0; g_hudlives_visible = 0; g_hudtime_visible = 0; }
void HUD_Update(void) {
    if (g_hudscore_visible) { if (g_score_display < g_game_score) { g_score_display += 100; if (g_score_display > g_game_score) g_score_display = g_game_score; } }
    if (g_hudrings_visible) { if (g_rings_display < g_game_rings) g_rings_display++; else if (g_rings_display > g_game_rings) g_rings_display--; }
}
void HUD_DrawAll(void) { HUD_DrawScore(); HUD_DrawRings(); HUD_DrawLives(); HUD_DrawTime(); }
void HUD_DrawScore(void) { if (g_hudscore_visible) Display_PrintFixed(200, 16, "SCORE"); }
void HUD_DrawRings(void) { if (g_hudrings_visible) Display_PrintFixed(200, 80, "RINGS"); }
void HUD_DrawLives(void) { if (g_hudlives_visible) Display_PrintFixed(200, 160, "LIVES"); }
void HUD_DrawTime(void) { if (g_hudtime_visible) Display_PrintFixed(200, 120, "TIME"); }
void HUD_Print(void) { }
void HUD_SetActive(u32 index, u32 value) { (void)index; (void)value; }
s32 HUD_IsActive(u32 index) { (void)index; return 1; }
u32 HUD_GetScore(void) { return g_game_score; }
u32 HUD_GetRings(void) { return g_game_rings; }
u32 HUD_GetLives(void) { return g_game_lives; }
u32 HUD_GetTime(void) { return g_game_time; }
void HUD_Hide(void) { g_hudscore_visible = 0; g_hudrings_visible = 0; g_hudlives_visible = 0; g_hudtime_visible = 0; }
void HUD_Show(void) { g_hudscore_visible = 1; g_hudrings_visible = 1; g_hudlives_visible = 1; g_hudtime_visible = 1; }
void HUD_Reset(void) { HUD_Init(); }
