/* Gameplay level enemy flying functions. */

#include "nds_types.h"

extern s32 Math_Sin(s32 angle);

void EnemyFlying_Init(void) { }

void EnemyFlying_Update(void) {
    u32 i;
    for (i = 0; i < g_enemy_count; i++) {
        if (!g_enemies[i].active || g_enemies[i].type != 1) continue;
        g_enemies[i].vy = Math_Sin(g_enemies[i].timer * 8) * 0x20;
    }
}

u32 EnemyFlying_GetCount(void) { return g_enemy_count; }
void EnemyFlying_Clear(void) { }
void EnemyFlying_Reset(void) { }
