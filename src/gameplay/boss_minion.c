/* Gameplay level boss minion functions. */

#include "nds_types.h"

#define BOSSMINION_MAX 8

static u32 s_bossminion_count;

void BossMinion_Init(void) { s_bossminion_count = 0; }

s32 BossMinion_Add(s32 x, s32 y, u32 param) {
    if (s_bossminion_count >= BOSSMINION_MAX) return -1;
    u32 i = s_bossminion_count;
    g_bossminions[i].x = x;
    g_bossminions[i].y = y;
    g_bossminions[i].type = param;
    g_bossminions[i].dir = 0;
    g_bossminions[i].hp = 1;
    g_bossminions[i].state = 0;
    g_bossminions[i].timer = 0;
    g_bossminions[i].active = 1;
    s_bossminion_count++;
    return i;
}

void BossMinion_Update(void) {
    u32 i;
    for (i = 0; i < s_bossminion_count; i++) {
        if (!g_bossminions[i].active) continue;
        g_bossminions[i].timer++;
        g_bossminions[i].x += (g_bossminions[i].dir == 0) ? 0x80 : -0x80;
    }
}

void BossMinion_Remove(u32 index) {
    if (index < s_bossminion_count) g_bossminions[index].active = 0;
}

u32 BossMinion_GetCount(void) { return s_bossminion_count; }

void BossMinion_Clear(void) { s_bossminion_count = 0; }
void BossMinion_Reset(void) { s_bossminion_count = 0; }
