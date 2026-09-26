#include "nds_types.h"

void HudTrick_Init(void) { g_hudtrick_visible = 0; g_hudtrick_timer = 0; }
void HudTrick_Update(void) {
    if (!g_hudtrick_visible) return;
    g_hudtrick_timer++;
    if (g_hudtrick_timer >= 60) { g_hudtrick_visible = 0; g_hudtrick_timer = 0; }
}
void HudTrick_Draw(void) { if (g_hudtrick_visible) Display_PrintFixed(g_hudtrick_x, g_hudtrick_y, "TRICK"); }
void HudTrick_Show(void) { g_hudtrick_visible = 1; g_hudtrick_timer = 0; }
void HudTrick_Hide(void) { g_hudtrick_visible = 0; }
s32 HudTrick_IsActive(void) { return g_hudtrick_visible; }
u32 HudTrick_GetTimer(void) { return g_hudtrick_timer; }
void HudTrick_Reset(void) { g_hudtrick_visible = 0; g_hudtrick_timer = 0; }
