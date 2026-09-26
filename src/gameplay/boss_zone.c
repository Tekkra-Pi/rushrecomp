/* Gameplay level boss zone functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);

void BossZone_Init(void) { g_bosszone_count = 0; }

s32 BossZone_Add(s32 x, s32 y, s32 w, s32 h, u32 boss_type) {
    u32 i = g_bosszone_count;
    g_bosszones[i].x = x;
    g_bosszones[i].y = y;
    g_bosszones[i].w = w;
    g_bosszones[i].h = h;
    g_bosszones[i].boss_type = boss_type;
    g_bosszones[i].triggered = 0;
    g_bosszones[i].active = 1;
    g_bosszone_count++;
    return i;
}

s32 BossZone_Check(void) {
    u32 i;
    for (i = 0; i < g_bosszone_count; i++) {
        if (!g_bosszones[i].active || g_bosszones[i].triggered) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        if (px >= g_bosszones[i].x && px <= g_bosszones[i].x + g_bosszones[i].w &&
            py >= g_bosszones[i].y && py <= g_bosszones[i].y + g_bosszones[i].h) {
            g_bosszones[i].triggered = 1;
            return i;
        }
    }
    return -1;
}

u32 BossZone_GetCount(void) { return g_bosszone_count; }

void BossZone_Clear(void) { g_bosszone_count = 0; }
void BossZone_Reset(void) {
    u32 i;
    for (i = 0; i < g_bosszone_count; i++) g_bosszones[i].triggered = 0;
}
