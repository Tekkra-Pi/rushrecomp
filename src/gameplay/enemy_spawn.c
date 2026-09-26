/* Gameplay level enemy spawn functions. */

#include "nds_types.h"

extern s32 Enemy_Spawn(s32 x, s32 y, u32 type, u32 dir);

void EnemySpawn_Init(void) { }

void EnemySpawn_Update(void) {
    u32 i;
    for (i = 0; i < g_enemy_count; i++) {
        if (!g_enemies[i].active) continue;
        s32 dx = g_enemies[i].x - g_camera_x;
        s32 dy = g_enemies[i].y - g_camera_y;
        if (dx < -0x2000 || dx > 0x2000 || dy < -0x2000 || dy > 0x2000) {
            g_enemies[i].active = 0;
        }
    }
}

u32 EnemySpawn_GetCount(void) { return g_enemy_count; }
void EnemySpawn_Clear(void) { }
void EnemySpawn_Reset(void) { }
