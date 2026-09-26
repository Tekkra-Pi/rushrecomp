/* Gameplay level enemy shoot functions. */

#include "nds_types.h"

void EnemyShoot_Init(void) { g_enemyproj_count = 0; }

s32 EnemyShoot_Add(s32 x, s32 y, u32 dir, u32 type) {
    if (g_enemyproj_count >= 16) return -1;
    u32 i = g_enemyproj_count;
    g_enemyprojs[i].x = x;
    g_enemyprojs[i].y = y;
    g_enemyprojs[i].vx = (dir == 0) ? 0x200 : -0x200;
    g_enemyprojs[i].vy = 0;
    g_enemyprojs[i].type = type;
    g_enemyprojs[i].timer = 0;
    g_enemyprojs[i].active = 1;
    g_enemyproj_count++;
    return i;
}

void EnemyShoot_Update(void) {
    u32 i;
    for (i = 0; i < g_enemyproj_count; i++) {
        if (!g_enemyprojs[i].active) continue;
        g_enemyprojs[i].x += g_enemyprojs[i].vx;
        g_enemyprojs[i].y += g_enemyprojs[i].vy;
        g_enemyprojs[i].timer++;
        if (g_enemyprojs[i].timer >= 180) {
            g_enemyprojs[i].active = 0;
        }
    }
}

void EnemyShoot_Remove(u32 index) {
    if (index < g_enemyproj_count) g_enemyprojs[index].active = 0;
}

u32 EnemyShoot_GetCount(void) { return g_enemyproj_count; }

void EnemyShoot_Clear(void) { g_enemyproj_count = 0; }
void EnemyShoot_Reset(void) { g_enemyproj_count = 0; }
