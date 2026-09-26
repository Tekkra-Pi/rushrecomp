/* Gameplay level enemy projectile functions. */
#include "nds_types.h"


void EnemyProj_Init(void) { g_enemyproj_count = 0; }

s32 EnemyProj_Add(s32 x, s32 y, s32 vx, s32 vy, u32 type) {
    if (g_enemyproj_count >= 16) return -1;
    u32 i = g_enemyproj_count;
    g_enemyprojs[i].x = x; g_enemyprojs[i].y = y;
    g_enemyprojs[i].vx = vx; g_enemyprojs[i].vy = vy;
    g_enemyprojs[i].type = type; g_enemyprojs[i].timer = 0;
    g_enemyprojs[i].active = 1;
    g_enemyproj_count++; return i;
}

void EnemyProj_Update(void) {
    u32 i;
    for (i = 0; i < g_enemyproj_count; i++) {
        if (!g_enemyprojs[i].active) continue;
        g_enemyprojs[i].x += g_enemyprojs[i].vx;
        g_enemyprojs[i].y += g_enemyprojs[i].vy;
        g_enemyprojs[i].timer++;
        if (g_enemyprojs[i].timer >= 120) { g_enemyprojs[i].active = 0; continue; }
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_enemyprojs[i].x;
        s32 dy = py - g_enemyprojs[i].y;
        if (dx >= -0x400 && dx <= 0x400 && dy >= -0x400 && dy <= 0x400 && !Player_IsInvincible()) {
            Player_Hurt();
            g_enemyprojs[i].active = 0;
        }
    }
}

void EnemyProj_Remove(u32 i) { if (i < g_enemyproj_count) g_enemyproj_count--; }
u32 EnemyProj_GetCount(void) { return g_enemyproj_count; }
void EnemyProj_Clear(void) { g_enemyproj_count = 0; }
void EnemyProj_Reset(void) { g_enemyproj_count = 0; }
