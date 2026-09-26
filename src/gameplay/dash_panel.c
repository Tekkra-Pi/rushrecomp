/* Gameplay level dash panel functions. */
#include "nds_types.h"


void DashPanel_Init(void) { g_dashpanel_count = 0; }

s32 DashPanel_Add(s32 x, s32 y, s32 w, s32 speed, u32 dir) {
    if (g_dashpanel_count >= 16) return -1;
    u32 i = g_dashpanel_count;
    g_dashpanels[i].x = x; g_dashpanels[i].y = y;
    g_dashpanels[i].w = w; g_dashpanels[i].speed = speed;
    g_dashpanels[i].dir = dir; g_dashpanels[i].active = 1;
    g_dashpanel_count++; return i;
}

void DashPanel_Update(void) {
    u32 i;
    for (i = 0; i < g_dashpanel_count; i++) {
        if (!g_dashpanels[i].active) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_dashpanels[i].x;
        if (dx >= 0 && dx <= g_dashpanels[i].w) {
            s32 dy = py - g_dashpanels[i].y;
            if (dy >= -0x400 && dy <= 0x400) {
                Player_SetVelocity(g_dashpanels[i].speed * g_dashpanels[i].dir, Player_GetVY());
                SoundEffect_Play(0xd0, 0x40, 0x100);
            }
        }
    }
}

void DashPanel_Remove(u32 i) { if (i < g_dashpanel_count) g_dashpanels[i].active = 0; }
u32 DashPanel_GetCount(void) { return g_dashpanel_count; }
void DashPanel_Clear(void) { g_dashpanel_count = 0; }
void DashPanel_Reset(void) { g_dashpanel_count = 0; }
