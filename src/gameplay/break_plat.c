/* Gameplay level breakable platform functions. */

#include "nds_types.h"

#define BREAKPLAT_MAX 16

void BreakPlat_Init(void) { g_breakplats[0].active = 0; }

s32 BreakPlat_Add(s32 x, s32 y, u32 param) {
    u32 i = 0;
    g_breakplats[i].x = x;
    g_breakplats[i].y = y;
    g_breakplats[i].w = 0x400;
    g_breakplats[i].timer = 0;
    g_breakplats[i].delay = param ? param : 30;
    g_breakplats[i].shaking = 0;
    g_breakplats[i].broken = 0;
    g_breakplats[i].active = 1;
    return i;
}

void BreakPlat_Update(void) {
    u32 i;
    for (i = 0; i < BREAKPLAT_MAX; i++) {
        if (!g_breakplats[i].active) continue;
        if (g_breakplats[i].shaking) {
            g_breakplats[i].timer++;
            if (g_breakplats[i].timer >= g_breakplats[i].delay) {
                g_breakplats[i].broken = 1;
                g_breakplats[i].shaking = 0;
                g_breakplats[i].active = 0;
            }
        }
    }
}

void BreakPlat_Trigger(void) {
    u32 i;
    for (i = 0; i < BREAKPLAT_MAX; i++) {
        if (!g_breakplats[i].active || g_breakplats[i].shaking) continue;
        g_breakplats[i].shaking = 1;
        g_breakplats[i].timer = 0;
    }
}

void BreakPlat_Remove(u32 index) {
    if (index < BREAKPLAT_MAX) g_breakplats[index].active = 0;
}

s32 BreakPlat_IsBroken(void) {
    u32 i;
    for (i = 0; i < BREAKPLAT_MAX; i++) {
        if (g_breakplats[i].broken) return 1;
    }
    return 0;
}

u32 BreakPlat_GetCount(void) { return BREAKPLAT_MAX; }

s32 BreakPlat_IsActive(u32 index) {
    if (index >= BREAKPLAT_MAX) return 0;
    return g_breakplats[index].active;
}

void BreakPlat_Clear(void) {
    u32 i;
    for (i = 0; i < BREAKPLAT_MAX; i++) g_breakplats[i].active = 0;
}

void BreakPlat_Reset(void) {
    u32 i;
    for (i = 0; i < BREAKPLAT_MAX; i++) {
        g_breakplats[i].active = 0;
        g_breakplats[i].broken = 0;
        g_breakplats[i].shaking = 0;
        g_breakplats[i].timer = 0;
    }
}
