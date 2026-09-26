/* Gameplay level smoke puff functions. */
#include "nds_types.h"

#define SMOKEPUFF_MAX 16

void SmokePuff_Init(void) { }

s32 SmokePuff_Spawn(s32 x, s32 y) {
    u32 i;
    for (i = 0; i < SMOKEPUFF_MAX; i++) {
        if (g_smokepuffs[i].active) continue;
        g_smokepuffs[i].x = x;
        g_smokepuffs[i].y = y;
        g_smokepuffs[i].vel_x = 0;
        g_smokepuffs[i].vel_y = -0x40;
        g_smokepuffs[i].timer = 0;
        g_smokepuffs[i].active = 1;
        return i;
    }
    return -1;
}

void SmokePuff_Update(void) {
    u32 i;
    for (i = 0; i < SMOKEPUFF_MAX; i++) {
        if (!g_smokepuffs[i].active) continue;
        g_smokepuffs[i].y += g_smokepuffs[i].vel_y;
        g_smokepuffs[i].timer++;
        if (g_smokepuffs[i].timer >= 20) g_smokepuffs[i].active = 0;
    }
}

void SmokePuff_Draw(void) { }

void SmokePuff_Remove(u32 index) {
    if (index < SMOKEPUFF_MAX) g_smokepuffs[index].active = 0;
}

u32 SmokePuff_GetCount(void) { return SMOKEPUFF_MAX; }

void SmokePuff_Clear(void) {
    u32 i;
    for (i = 0; i < SMOKEPUFF_MAX; i++) g_smokepuffs[i].active = 0;
}

void SmokePuff_Reset(void) { SmokePuff_Clear(); }
