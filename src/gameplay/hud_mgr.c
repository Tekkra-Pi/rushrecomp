/* Gameplay level HUD manager functions. */

#include "nds_types.h"

static u32 s_hudmgr_state;

void HudMgr_Init(void) {
    s_hudmgr_state = 0;
    g_hud_update_flags = 0;
    g_hud_update_timer = 0;
}

void HudMgr_Update(void) {
    g_hud_update_timer++;
    if (g_hud_update_timer >= 2) {
        g_hud_update_timer = 0;
        g_hud_update_flags = 1;
    }
}

void HudMgr_Show(void) {
    g_hudscore_visible = 1;
    g_hudrings_visible = 1;
    g_hudtime_visible = 1;
    g_hudboost_visible = 1;
    g_hudlives_visible = 1;
}

void HudMgr_Hide(void) {
    g_hudscore_visible = 0;
    g_hudrings_visible = 0;
    g_hudtime_visible = 0;
    g_hudboost_visible = 0;
    g_hudlives_visible = 0;
}

u32 HudMgr_GetFlags(void) {
    u32 f = g_hud_update_flags;
    g_hud_update_flags = 0;
    return f;
}

void HudMgr_Reset(void) {
    s_hudmgr_state = 0;
    g_hud_update_flags = 0;
    g_hud_update_timer = 0;
}
