/* Gameplay level enemy actor functions. */

#include "nds_types.h"

#define ENEMYACTOR_MAX 16

void EnemyActor_Init(void) { g_enemyactor_count = 0; }

s32 EnemyActor_Add(s32 x, s32 y, u32 param) {
    if (g_enemyactor_count >= ENEMYACTOR_MAX) return -1;
    u32 i = g_enemyactor_count;
    g_enemyactors[i].x = x;
    g_enemyactors[i].y = y;
    g_enemyactors[i].vx = 0;
    g_enemyactors[i].vy = 0;
    g_enemyactors[i].type = param;
    g_enemyactors[i].dir = 0;
    g_enemyactors[i].hp = 1;
    g_enemyactors[i].state = 0;
    g_enemyactors[i].timer = 0;
    g_enemyactors[i].active = 1;
    g_enemyactor_count++;
    return i;
}

void EnemyActor_Update(void) {
    u32 i;
    for (i = 0; i < g_enemyactor_count; i++) {
        if (!g_enemyactors[i].active) continue;
        g_enemyactors[i].x += g_enemyactors[i].vx;
        g_enemyactors[i].y += g_enemyactors[i].vy;
        g_enemyactors[i].timer++;
    }
}

void EnemyActor_Remove(u32 index) {
    if (index < g_enemyactor_count) g_enemyactors[index].active = 0;
}

u32 EnemyActor_GetCount(void) { return g_enemyactor_count; }
void EnemyActor_Clear(void) { g_enemyactor_count = 0; }
void EnemyActor_Reset(void) { g_enemyactor_count = 0; }
