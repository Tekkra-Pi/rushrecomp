/* Gameplay level boss projectile functions. */

#include "nds_types.h"

#define BOSSPROJ_MAX 16

void BossProj_Init(void) { g_bossproj_count = 0; }

s32 BossProj_Add(s32 x, s32 y, u32 param) {
    if (g_bossproj_count >= BOSSPROJ_MAX) return -1;
    u32 i = g_bossproj_count;
    g_enemyprojs[i].x = x;
    g_enemyprojs[i].y = y;
    g_enemyprojs[i].vx = 0;
    g_enemyprojs[i].vy = 0x200;
    g_enemyprojs[i].type = param;
    g_enemyprojs[i].timer = 0;
    g_enemyprojs[i].active = 1;
    g_bossproj_count++;
    return i;
}

void BossProj_Update(void) {
    u32 i;
    for (i = 0; i < g_bossproj_count; i++) {
        if (!g_enemyprojs[i].active) continue;
        g_enemyprojs[i].x += g_enemyprojs[i].vx;
        g_enemyprojs[i].y += g_enemyprojs[i].vy;
        g_enemyprojs[i].timer++;
        if (g_enemyprojs[i].timer >= 180) {
            g_enemyprojs[i].active = 0;
        }
    }
}

void BossProj_Remove(u32 index) {
    if (index < g_bossproj_count) g_enemyprojs[index].active = 0;
}

u32 BossProj_GetCount(void) { return g_bossproj_count; }

void BossProj_Clear(void) { g_bossproj_count = 0; }
void BossProj_Reset(void) { g_bossproj_count = 0; }
