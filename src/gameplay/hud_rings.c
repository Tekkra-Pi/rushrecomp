#include "nds_types.h"

void HudRings_Init(void) { g_hudrings_visible = 0; g_rings_display = 0; }
void HudRings_Update(void) {
    if (!g_hudrings_visible) return;
    if (g_rings_display < g_game_rings) g_rings_display++;
    else if (g_rings_display > g_game_rings) g_rings_display--;
}
void HudRings_Draw(void) { if (g_hudrings_visible) Display_PrintFixed(200, 80, "RINGS"); }
void HudRings_Show(void) { g_hudrings_visible = 1; }
void HudRings_Hide(void) { g_hudrings_visible = 0; }
s32 HudRings_IsActive(void) { return g_hudrings_visible; }
u32 HudRings_GetValue(void) { return g_rings_display; }
void HudRings_Reset(void) { g_hudrings_visible = 0; g_rings_display = 0; }
