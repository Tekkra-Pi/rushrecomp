/* Gameplay level slide dust functions. */
#include "nds_types.h"

#define SLIDEDUST_MAX 8

void SlideDust_Init(void) { }

s32 SlideDust_Spawn(s32 x, s32 y) {
    u32 i;
    for (i = 0; i < SLIDEDUST_MAX; i++) {
        if (g_slidedusts[i].active) continue;
        g_slidedusts[i].x = x;
        g_slidedusts[i].y = y;
        g_slidedusts[i].vel_x = 0;
        g_slidedusts[i].vel_y = -0x40;
        g_slidedusts[i].timer = 0;
        g_slidedusts[i].active = 1;
        return i;
    }
    return -1;
}

void SlideDust_Update(void) {
    u32 i;
    for (i = 0; i < SLIDEDUST_MAX; i++) {
        if (!g_slidedusts[i].active) continue;
        g_slidedusts[i].y += g_slidedusts[i].vel_y;
        g_slidedusts[i].timer++;
        if (g_slidedusts[i].timer >= 16) g_slidedusts[i].active = 0;
    }
}

void SlideDust_Draw(void) { }

u32 SlideDust_GetCount(void) { return SLIDEDUST_MAX; }

void SlideDust_Clear(void) {
    u32 i;
    for (i = 0; i < SLIDEDUST_MAX; i++) g_slidedusts[i].active = 0;
}

void SlideDust_Reset(void) { SlideDust_Clear(); }
