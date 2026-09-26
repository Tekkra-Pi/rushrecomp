#include "nds_types.h"
void TrickNameDisplay_Init(void) { g_tricknamedisp_visible = 0; g_tricknamedisp_timer = 0; }
void TrickNameDisplay_SetPosition(u32 index, u32 value) { (void)index; g_tricknamedisp_x = (s32)value; }
void TrickNameDisplay_Start(void) { g_tricknamedisp_visible = 1; g_tricknamedisp_timer = 0; }
void TrickNameDisplay_Update(void) {
    if (!g_tricknamedisp_visible) return;
    g_tricknamedisp_timer++;
    if (g_tricknamedisp_timer >= 60) { g_tricknamedisp_visible = 0; }
}
void TrickNameDisplay_Draw(void) {
    if (g_tricknamedisp_visible)
        Display_PrintFixed(g_tricknamedisp_x, g_tricknamedisp_y, "TRICK");
}
void TrickNameDisplay_Stop(void) { g_tricknamedisp_visible = 0; }
s32 TrickNameDisplay_IsVisible(void) { return g_tricknamedisp_visible; }
u32 TrickNameDisplay_GetTimer(void) { return g_tricknamedisp_timer; }
void TrickNameDisplay_Reset(void) { g_tricknamedisp_visible = 0; g_tricknamedisp_timer = 0; }
