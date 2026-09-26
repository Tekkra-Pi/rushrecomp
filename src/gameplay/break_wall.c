/* Gameplay level break wall functions. */

#include "nds_types.h"

extern void ExplosionFX_Spawn(s32 x, s32 y);

#define BREAKWALL_MAX 16

void BreakWall_Init(void) { g_breakwalls[0].active = 0; }

s32 BreakWall_Add(s32 x, s32 y, u32 param) {
    u32 i = 0;
    g_breakwalls[i].x = x;
    g_breakwalls[i].y = y;
    g_breakwalls[i].timer = 0;
    g_breakwalls[i].broken = 0;
    g_breakwalls[i].active = 1;
    return i;
}

void BreakWall_Update(void) {
    u32 i;
    for (i = 0; i < BREAKWALL_MAX; i++) {
        if (!g_breakwalls[i].active) continue;
        if (g_breakwalls[i].broken) {
            g_breakwalls[i].timer++;
            if (g_breakwalls[i].timer >= 30) {
                g_breakwalls[i].active = 0;
            }
        }
    }
}

void BreakWall_Hit(u32 index) {
    if (index >= BREAKWALL_MAX) return;
    if (g_breakwalls[index].active && !g_breakwalls[index].broken) {
        g_breakwalls[index].broken = 1;
        g_breakwalls[index].timer = 0;
        ExplosionFX_Spawn(g_breakwalls[index].x, g_breakwalls[index].y);
    }
}

void BreakWall_Remove(u32 index) {
    if (index < BREAKWALL_MAX) g_breakwalls[index].active = 0;
}

u32 BreakWall_GetCount(void) { return BREAKWALL_MAX; }

void BreakWall_Clear(void) {
    u32 i;
    for (i = 0; i < BREAKWALL_MAX; i++) g_breakwalls[i].active = 0;
}

void BreakWall_Reset(void) {
    u32 i;
    for (i = 0; i < BREAKWALL_MAX; i++) {
        g_breakwalls[i].active = 0;
        g_breakwalls[i].broken = 0;
        g_breakwalls[i].timer = 0;
    }
}
