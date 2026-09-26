/* Gameplay level enemy shooting functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);

void EnemyShooting_Init(void) { }

void EnemyShooting_Update(void) {
    u32 i;
    for (i = 0; i < g_enemy_count; i++) {
        if (!g_enemies[i].active) continue;
        if (g_enemies[i].timer % 120 == 0) {
            s32 px = Player_GetX();
            u32 dir = (px < g_enemies[i].x) ? 1 : 0;
            EnemyShoot_Add(g_enemies[i].x, g_enemies[i].y, dir, 0);
        }
    }
}

void EnemyShooting_Clear(void) { }
void EnemyShooting_Reset(void) { }
