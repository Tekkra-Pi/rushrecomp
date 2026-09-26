/* Gameplay level enemy core functions. */
#include "nds_types.h"
#include "entity.h"

u32 Enemy_GetHP(u32 type);
s32 Math_Sin(s32 angle);

void Enemy_Init(void) { g_enemy_count = 0; }

s32 Enemy_Spawn(s32 x, s32 y, u32 type, u32 dir) {
    if (g_enemy_count >= 32) return -1;
    u32 i = g_enemy_count;
    g_enemies[i].x = x; g_enemies[i].y = y;
    g_enemies[i].type = type; g_enemies[i].dir = dir;
    g_enemies[i].vx = 0; g_enemies[i].vy = 0;
    g_enemies[i].hp = Enemy_GetHP(type);
    g_enemies[i].state = 0; g_enemies[i].timer = 0;
    g_enemies[i].active = 1;
    g_enemy_count++; return i;
}

u32 Enemy_GetHP(u32 type) {
    switch (type) {
        case 0: return 1;
        case 1: return 2;
        case 2: return 3;
        default: return 1;
    }
}

void Enemy_UpdateBehavior(u32 i) {
    if (i >= g_enemy_count || !g_enemies[i].active) return;
    switch (g_enemies[i].type) {
        case 0: /* walker */
            g_enemies[i].vx = g_enemies[i].dir ? -0x40 : 0x40;
            break;
        case 1: /* flyer */
            g_enemies[i].vy = Math_Sin(g_enemies[i].timer * 8) * 0x20;
            break;
        case 2: /* stomper */
            if (g_enemies[i].timer % 60 == 0) g_enemies[i].vy = -0x200;
            break;
    }
}

void Enemy_Kill(u32 i) {
    if (i < g_enemy_count && g_enemies[i].active) {
        g_enemies[i].active = 0;
        ExplosionFX_Spawn(g_enemies[i].x, g_enemies[i].y);
        Player_AddScore(100);
    }
}

u32 Enemy_GetCount(void) { return g_enemy_count; }
void Enemy_Remove(u32 i) { if (i < g_enemy_count) g_enemy_count--; }
void Enemy_Clear(void) { g_enemy_count = 0; }
void Enemy_Reset(void) { g_enemy_count = 0; }
