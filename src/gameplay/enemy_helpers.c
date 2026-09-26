/* Gameplay level enemy helpers functions. */

#include "nds_types.h"

extern void Player_GetPosition(s32 *x, s32 *y);

void EnemyHelpers_Init(void) { }

s32 EnemyHelpers_GetDistance(u32 index) {
    if (index >= g_enemy_count) return 0x7FFFFFFF;
    s32 px, py;
    Player_GetPosition(&px, &py);
    s32 dx = px - g_enemies[index].x;
    s32 dy = py - g_enemies[index].y;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return dx + dy;
}

s32 EnemyHelpers_GetDirection(u32 index) {
    if (index >= g_enemy_count) return 0;
    s32 px, py;
    Player_GetPosition(&px, &py);
    return (px < g_enemies[index].x) ? 0 : 1;
}

void EnemyHelpers_Update(void) { }

u32 EnemyHelpers_GetCount(void) { return g_enemy_count; }

void EnemyHelpers_Clear(void) { }
void EnemyHelpers_Reset(void) { }
