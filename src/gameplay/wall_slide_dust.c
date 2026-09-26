/* Gameplay level wall slide dust functions. */
#include "nds_types.h"

#define WALLSLIDEDUST_MAX 8

void WallSlideDust_Init(void) { }

s32 WallSlideDust_Spawn(s32 x, s32 y) {
    u32 i;
    for (i = 0; i < WALLSLIDEDUST_MAX; i++) {
        if (g_wallsliedusts[i].active) continue;
        g_wallsliedusts[i].x = x;
        g_wallsliedusts[i].y = y;
        g_wallsliedusts[i].vel_x = 0;
        g_wallsliedusts[i].vel_y = -0x40;
        g_wallsliedusts[i].timer = 0;
        g_wallsliedusts[i].active = 1;
        return i;
    }
    return -1;
}

void WallSlideDust_Update(void) {
    u32 i;
    for (i = 0; i < WALLSLIDEDUST_MAX; i++) {
        if (!g_wallsliedusts[i].active) continue;
        g_wallsliedusts[i].y += g_wallsliedusts[i].vel_y;
        g_wallsliedusts[i].timer++;
        if (g_wallsliedusts[i].timer >= 16) g_wallsliedusts[i].active = 0;
    }
}

void WallSlideDust_Draw(void) { }

void WallSlideDust_Clear(void) {
    u32 i;
    for (i = 0; i < WALLSLIDEDUST_MAX; i++) g_wallsliedusts[i].active = 0;
}

void WallSlideDust_Reset(void) { WallSlideDust_Clear(); }
