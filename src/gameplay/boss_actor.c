/* Gameplay level boss actor functions. */

#include "nds_types.h"

extern void Player_GetPosition(s32 *x, s32 *y);

#define BOSSACTOR_MAX 4

void BossActor_Init(void) { g_bossactor_count = 0; }

s32 BossActor_Add(s32 x, s32 y, u32 param) {
    if (g_bossactor_count >= BOSSACTOR_MAX) return -1;
    u32 i = g_bossactor_count;
    g_bossactors[i].x = x;
    g_bossactors[i].y = y;
    g_bossactors[i].vx = 0;
    g_bossactors[i].vy = 0;
    g_bossactors[i].hp = 10;
    g_bossactors[i].max_hp = 10;
    g_bossactors[i].boss_type = param;
    g_bossactors[i].state = 0;
    g_bossactors[i].timer = 0;
    g_bossactors[i].active = 1;
    g_bossactor_count++;
    return i;
}

void BossActor_Update(void) {
    u32 i;
    for (i = 0; i < g_bossactor_count; i++) {
        if (!g_bossactors[i].active) continue;
        g_bossactors[i].x += g_bossactors[i].vx;
        g_bossactors[i].y += g_bossactors[i].vy;
        g_bossactors[i].timer++;
    }
}

s32 BossActor_GetHP(u32 index) {
    if (index >= g_bossactor_count) return 0;
    return g_bossactors[index].hp;
}

s32 BossActor_GetMaxHP(u32 index) {
    if (index >= g_bossactor_count) return 0;
    return g_bossactors[index].max_hp;
}

s32 BossActor_GetX(u32 index) {
    if (index >= g_bossactor_count) return 0;
    return g_bossactors[index].x;
}

s32 BossActor_GetY(u32 index) {
    if (index >= g_bossactor_count) return 0;
    return g_bossactors[index].y;
}

u32 BossActor_GetCount(void) { return g_bossactor_count; }

void BossActor_Remove(u32 index) {
    if (index < g_bossactor_count) g_bossactors[index].active = 0;
}

void BossActor_Clear(void) { g_bossactor_count = 0; }
void BossActor_Reset(void) { g_bossactor_count = 0; }
