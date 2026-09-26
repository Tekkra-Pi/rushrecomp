/* Gameplay level enemy group functions. */

#include "nds_types.h"

void EnemyGroup_Init(void) { }

s32 EnemyGroup_GetCount(void) { return g_enemy_count; }

void EnemyGroup_Update(void) {
    u32 i;
    for (i = 0; i < g_enemy_count; i++) {
        if (!g_enemies[i].active) continue;
        g_enemies[i].x += g_enemies[i].vx;
        g_enemies[i].y += g_enemies[i].vy;
    }
}

void EnemyGroup_Remove(u32 index) {
    if (index < g_enemy_count) g_enemies[index].active = 0;
}

void EnemyGroup_Clear(void) { g_enemy_count = 0; }
void EnemyGroup_Reset(void) { g_enemy_count = 0; }
