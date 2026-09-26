/* Gameplay level enemy death FX functions. */

#include "nds_types.h"

extern void ExplosionFX_Spawn(s32 x, s32 y);
extern void Spark_Spawn(s32 x, s32 y);
extern void Player_AddScore(u32 score);

#define ENEMYDEATHFX_MAX 16

void EnemyDeathFX_Init(void) { }

void EnemyDeathFX_Update(void) {
    u32 i;
    for (i = 0; i < ENEMYDEATHFX_MAX; i++) {
        if (!g_enemydeaths[i].active) continue;
        g_enemydeaths[i].timer++;
        g_enemydeaths[i].y -= 0x100;
        if (g_enemydeaths[i].timer >= 30) {
            g_enemydeaths[i].active = 0;
        }
    }
}

void EnemyDeathFX_Spawn(s32 x, s32 y) {
    u32 i;
    for (i = 0; i < ENEMYDEATHFX_MAX; i++) {
        if (g_enemydeaths[i].active) continue;
        g_enemydeaths[i].x = x;
        g_enemydeaths[i].y = y;
        g_enemydeaths[i].timer = 0;
        g_enemydeaths[i].active = 1;
        ExplosionFX_Spawn(x, y);
        Spark_Spawn(x, y);
        Player_AddScore(100);
        return;
    }
}

u32 EnemyDeathFX_GetCount(void) {
    u32 count = 0;
    u32 i;
    for (i = 0; i < ENEMYDEATHFX_MAX; i++) {
        if (g_enemydeaths[i].active) count++;
    }
    return count;
}

void EnemyDeathFX_Clear(void) {
    u32 i;
    for (i = 0; i < ENEMYDEATHFX_MAX; i++) g_enemydeaths[i].active = 0;
}

void EnemyDeathFX_Reset(void) { EnemyDeathFX_Clear(); }
