/* Gameplay level HUD position display functions. */
#include "nds_types.h"


void HudPos_Init(void) { g_hudpos_x = 200; g_hudpos_y = 200; g_hudpos_active = 0; }

void HudPos_Draw(void) {
    if (!g_hudpos_active) return;
    s32 px, py;
    Player_GetPosition(&px, &py);
    char buf[32];
    sprintf(buf, "%d,%d", px, py);
    Display_PrintFixed(g_hudpos_x, g_hudpos_y, buf);
}

void HudPos_SetPos(s32 x, s32 y) { g_hudpos_x = x; g_hudpos_y = y; }
void HudPos_Show(void) { g_hudpos_active = 1; }
void HudPos_Hide(void) { g_hudpos_active = 0; }
s32 HudPos_IsVisible(void) { return g_hudpos_active; }
void HudPos_Reset(void) { g_hudpos_x = 200; g_hudpos_y = 200; g_hudpos_active = 0; }
