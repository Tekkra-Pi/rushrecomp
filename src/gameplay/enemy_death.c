/* Gameplay level enemy death functions. */

#include "nds_types.h"

extern void ExplosionFX_Spawn(s32 x, s32 y);

void EnemyDeath_Init(void) { g_enemy_count = 0; }

void EnemyDeath_Update(void) {
    u32 i;
    for (i = 0; i < g_enemy_count; i++) {
        if (!g_enemydeaths[i].active) continue;
        g_enemydeaths[i].timer++;
        if (g_enemydeaths[i].timer >= 30) {
            g_enemydeaths[i].active = 0;
        }
    }
}

void EnemyDeath_Spawn(s32 x, s32 y) {
    u32 i;
    for (i = 0; i < 16; i++) {
        if (g_enemydeaths[i].active) continue;
        g_enemydeaths[i].x = x;
        g_enemydeaths[i].y = y;
        g_enemydeaths[i].timer = 0;
        g_enemydeaths[i].active = 1;
        return;
    }
}

void EnemyDeath_Remove(u32 index) {
    if (index < 16) g_enemydeaths[index].active = 0;
}

u32 EnemyDeath_GetCount(void) { return g_enemy_count; }

void EnemyDeath_Clear(void) {
    u32 i;
    for (i = 0; i < 16; i++) g_enemydeaths[i].active = 0;
}

void EnemyDeath_Reset(void) {
    EnemyDeath_Clear();
}
