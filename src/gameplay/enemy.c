/* Enemy system functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern void Player_GetPosition(s32 *x, s32 *y);

#define ENEMY_MAX 32

void Enemy_Init(void) { g_enemy_count = 0; }

s32 Enemy_Add(s32 x, s32 y, u32 type, u32 dir) {
    if (g_enemy_count >= ENEMY_MAX) return -1;
    u32 i = g_enemy_count;
    g_enemies[i].x = x;
    g_enemies[i].y = y;
    g_enemies[i].type = type;
    g_enemies[i].dir = dir;
    g_enemies[i].vx = 0;
    g_enemies[i].vy = 0;
    g_enemies[i].hp = 1;
    g_enemies[i].state = 0;
    g_enemies[i].timer = 0;
    g_enemies[i].active = 1;
    g_enemy_count++;
    return i;
}

void Enemy_Update(void) {
    u32 i;
    for (i = 0; i < g_enemy_count; i++) {
        if (!g_enemies[i].active) continue;
        g_enemies[i].x += g_enemies[i].vx;
        g_enemies[i].y += g_enemies[i].vy;
        g_enemies[i].timer++;
    }
}

void Enemy_Remove(u32 index) {
    if (index < g_enemy_count) g_enemies[index].active = 0;
}

u32 Enemy_GetCount(void) { return g_enemy_count; }

void Enemy_Clear(void) { g_enemy_count = 0; }
void Enemy_Reset(void) { g_enemy_count = 0; }
