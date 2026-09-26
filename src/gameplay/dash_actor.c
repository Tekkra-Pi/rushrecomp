/* Gameplay level dash panel actor functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);
extern void Player_SetVX(s32 vx);

#define DASHACTOR_MAX 16

void DashActor_Init(void) { g_dashactors = 0; }

s32 DashActor_Add(s32 x, s32 y, u32 param) {
    u32 i = g_dashactors;
    g_dashpanels[i].x = x;
    g_dashpanels[i].y = y;
    g_dashpanels[i].w = 0x400;
    g_dashpanels[i].speed = 0x600;
    g_dashpanels[i].dir = param;
    g_dashpanels[i].active = 1;
    g_dashactors++;
    return i;
}

void DashActor_Update(void) {
    u32 i;
    for (i = 0; i < g_dashactors; i++) {
        if (!g_dashpanels[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - g_dashpanels[i].x;
        s32 dy = py - g_dashpanels[i].y;
        if (dx >= -0x200 && dx <= g_dashpanels[i].w + 0x200 &&
            dy >= -0x200 && dy <= 0x200) {
            Player_SetVX(g_dashpanels[i].speed);
        }
    }
}

void DashActor_Remove(u32 index) {
    if (index < g_dashactors) g_dashpanels[index].active = 0;
}

s32 DashActor_GetX(u32 index) {
    if (index >= g_dashactors) return 0;
    return g_dashpanels[index].x;
}

s32 DashActor_GetY(u32 index) {
    if (index >= g_dashactors) return 0;
    return g_dashpanels[index].y;
}

u32 DashActor_GetCount(void) { return g_dashactors; }

s32 DashActor_IsActive(u32 index) {
    if (index >= g_dashactors) return 0;
    return g_dashpanels[index].active;
}

void DashActor_Clear(void) { g_dashactors = 0; }
void DashActor_Reset(void) { g_dashactors = 0; }
