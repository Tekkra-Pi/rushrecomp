/* Gameplay level boss laser functions. */

#include "nds_types.h"

static u32 s_bosslaser_count;
static BossMinionEntry s_bosslasers[8];

void BossLaser_Init(void) { s_bosslaser_count = 0; }

s32 BossLaser_Add(s32 x, s32 y, u32 param) {
    if (s_bosslaser_count >= 8) return -1;
    u32 i = s_bosslaser_count;
    s_bosslasers[i].x = x;
    s_bosslasers[i].y = y;
    s_bosslasers[i].type = param;
    s_bosslasers[i].dir = 0;
    s_bosslasers[i].hp = 1;
    s_bosslasers[i].state = 0;
    s_bosslasers[i].timer = 0;
    s_bosslasers[i].active = 1;
    s_bosslaser_count++;
    return i;
}

void BossLaser_Update(void) {
    u32 i;
    for (i = 0; i < s_bosslaser_count; i++) {
        if (!s_bosslasers[i].active) continue;
        s_bosslasers[i].timer++;
        if (s_bosslasers[i].timer >= 120) {
            s_bosslasers[i].active = 0;
        }
    }
}

void BossLaser_Draw(void) {
}

void BossLaser_Remove(u32 index) {
    if (index < s_bosslaser_count) s_bosslasers[index].active = 0;
}

u32 BossLaser_GetCount(void) { return s_bosslaser_count; }

void BossLaser_Clear(void) { s_bosslaser_count = 0; }
void BossLaser_Reset(void) { s_bosslaser_count = 0; }
