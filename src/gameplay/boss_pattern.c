/* Gameplay level boss pattern functions. */

#include "nds_types.h"

#define BOSSPATTERN_MAX 8

static u32 s_bosspattern_count;

void BossPattern_Init(void) { s_bosspattern_count = 0; }

s32 BossPattern_Add(s32 x, s32 y, u32 param) {
    if (s_bosspattern_count >= BOSSPATTERN_MAX) return -1;
    u32 i = s_bosspattern_count;
    s_bosspattern_count++;
    return i;
}

void BossPattern_Update(void) {
}

void BossPattern_Remove(u32 index) {
    (void)index;
}

u32 BossPattern_GetCount(void) { return s_bosspattern_count; }

void BossPattern_Clear(void) { s_bosspattern_count = 0; }
void BossPattern_Reset(void) { s_bosspattern_count = 0; }
