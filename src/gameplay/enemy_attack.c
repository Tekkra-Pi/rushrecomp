/* Gameplay level enemy attack functions. */

#include "nds_types.h"

extern void Player_GetPosition(s32 *x, s32 *y);

void EnemyAttack_Init(void) { }

s32 EnemyAttack_Check(void) {
    u32 i;
    for (i = 0; i < g_enemy_count; i++) {
        if (!g_enemies[i].active) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_enemies[i].x;
        s32 dy = py - g_enemies[i].y;
        if (dx >= -0x300 && dx <= 0x300 && dy >= -0x300 && dy <= 0x300) {
            return 1;
        }
    }
    return 0;
}

void EnemyAttack_Apply(void) {
}

u32 EnemyAttack_GetCount(void) { return g_enemy_count; }

void EnemyAttack_Clear(void) { }
void EnemyAttack_Reset(void) { }
