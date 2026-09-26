/* Gameplay level boss trigger functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);

#define BOSSTRIG_MAX 8

void BossTrigger_Init(void) { }

s32 BossTrigger_Add(s32 x, s32 y, u32 param) {
    u32 i = g_bosszone_count;
    g_bosstrigs[i].x = x;
    g_bosstrigs[i].y = y;
    g_bosstrigs[i].boss_id = param;
    g_bosstrigs[i].triggered = 0;
    g_bosstrigs[i].active = 1;
    return i;
}

void BossTrigger_Remove(u32 index) {
    if (index < g_bosszone_count) g_bosstrigs[index].active = 0;
}

s32 BossTrigger_Check(void) {
    u32 i;
    for (i = 0; i < g_bosszone_count; i++) {
        if (!g_bosstrigs[i].active || g_bosstrigs[i].triggered) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - g_bosstrigs[i].x;
        s32 dy = py - g_bosstrigs[i].y;
        if (dx >= -0x400 && dx <= 0x400 && dy >= -0x400 && dy <= 0x400) {
            g_bosstrigs[i].triggered = 1;
            return i;
        }
    }
    return -1;
}

u32 BossTrigger_GetBossId(void) {
    u32 i;
    for (i = 0; i < g_bosszone_count; i++) {
        if (g_bosstrigs[i].triggered) return g_bosstrigs[i].boss_id;
    }
    return 0;
}

s32 BossTrigger_IsTriggered(void) {
    u32 i;
    for (i = 0; i < g_bosszone_count; i++) {
        if (g_bosstrigs[i].triggered) return 1;
    }
    return 0;
}

u32 BossTrigger_GetCount(void) { return g_bosszone_count; }

void BossTrigger_Clear(void) { g_bosszone_count = 0; }
void BossTrigger_Reset(void) {
    u32 i;
    for (i = 0; i < g_bosszone_count; i++) {
        g_bosstrigs[i].triggered = 0;
    }
}
