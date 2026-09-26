#include "nds_types.h"
void TrickUI_Init(void) { g_trickui_active = 0; g_trickui_timer = 0; g_trickui_score = 0; g_trickui_trick_id = 0; }
void TrickUI_Start(void) { g_trickui_active = 1; g_trickui_timer = 0; }
void TrickUI_Update(void) {
    if (!g_trickui_active) return;
    g_trickui_timer++;
    if (g_trickui_timer >= 60) g_trickui_active = 0;
}
void TrickUI_Draw(void) {
    if (!g_trickui_active) return;
    Display_PrintFixed(g_hudtrick_x, g_hudtrick_y, "TRICK");
}
void TrickUI_Stop(void) { g_trickui_active = 0; }
s32 TrickUI_IsActive(u32 index) { (void)index; return g_trickui_active; }
u32 TrickUI_GetTrickId(void) { return g_trickui_trick_id; }
u32 TrickUI_GetScore(void) { return g_trickui_score; }
u32 TrickUI_GetTimer(void) { return g_trickui_timer; }
void TrickUI_Reset(void) { g_trickui_active = 0; g_trickui_timer = 0; g_trickui_score = 0; g_trickui_trick_id = 0; }
