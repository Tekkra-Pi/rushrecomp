#include "nds_types.h"

void HudLives_Init(void) { g_hudlives_visible = 0; g_hud_lives = 0; }
void HudLives_Update(void) { if (g_hudlives_visible) g_hud_lives = g_game_lives; }
void HudLives_Draw(void) { if (g_hudlives_visible) Display_PrintFixed(200, 160, "LIVES"); }
void HudLives_Show(void) { g_hudlives_visible = 1; }
void HudLives_Hide(void) { g_hudlives_visible = 0; }
s32 HudLives_IsActive(void) { return g_hudlives_visible; }
u32 HudLives_GetValue(void) { return g_hud_lives; }
void HudLives_Reset(void) { g_hudlives_visible = 0; g_hud_lives = 0; }
