#include "nds_types.h"
static u32 s_vis, s_sel;
void PauseScreen_Init(void) { s_vis = 0; s_sel = 0; }
void PauseScreen_Show(void) { s_vis = 1; s_sel = 0; }
void PauseScreen_Hide(void) { s_vis = 0; }
s32 PauseScreen_Update(void) {
    if (!s_vis) return 0;
    if (g_key_pressed & (1 << 5)) { if (s_sel > 0) s_sel--; }
    if (g_key_pressed & (1 << 4)) { if (s_sel < 2) s_sel++; }
    if (g_key_pressed & (1 << 0)) return 1;
    return 0;
}
void PauseScreen_Draw(void) {
    if (!s_vis) return;
    Display_PrintFixed(100, 60, "PAUSED");
    Display_PrintFixed(100, 80, "RESUME");
    Display_PrintFixed(100, 100, "RETRY");
    Display_PrintFixed(100, 120, "QUIT");
    Display_PrintFixed(80, 80 + s_sel * 20, ">");
}
s32 PauseScreen_IsVisible(void) { return s_vis; }
u32 PauseScreen_GetSelection(void) { return s_sel; }
void PauseScreen_Reset(void) { s_vis = 0; s_sel = 0; }
