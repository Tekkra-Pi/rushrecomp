#include "nds_types.h"

void HudTime_Init(void) { g_hudtime_visible = 0; }
void HudTime_Update(void) { }
void HudTime_Draw(void) { if (g_hudtime_visible) Display_PrintFixed(200, 120, "TIME"); }
void HudTime_Show(void) { g_hudtime_visible = 1; }
void HudTime_Hide(void) { g_hudtime_visible = 0; }
s32 HudTime_IsActive(void) { return g_hudtime_visible; }
u32 HudTime_GetValue(void) { return g_game_time; }
void HudTime_Reset(void) { g_hudtime_visible = 0; }
