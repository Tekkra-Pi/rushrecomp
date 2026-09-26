/* Gameplay level spindash dust functions. */
#include "nds_types.h"

#define SPINDASHDUST_MAX 8

void SpindashDust_Init(void) { }

s32 SpindashDust_Spawn(s32 x, s32 y) {
    u32 i;
    for (i = 0; i < SPINDASHDUST_MAX; i++) {
        if (g_spindashdusts[i].active) continue;
        g_spindashdusts[i].x = x;
        g_spindashdusts[i].y = y;
        g_spindashdusts[i].vel_x = 0;
        g_spindashdusts[i].vel_y = -0x60;
        g_spindashdusts[i].timer = 0;
        g_spindashdusts[i].active = 1;
        return i;
    }
    return -1;
}

void SpindashDust_Update(void) {
    u32 i;
    for (i = 0; i < SPINDASHDUST_MAX; i++) {
        if (!g_spindashdusts[i].active) continue;
        g_spindashdusts[i].y += g_spindashdusts[i].vel_y;
        g_spindashdusts[i].timer++;
        if (g_spindashdusts[i].timer >= 12) g_spindashdusts[i].active = 0;
    }
}

void SpindashDust_Draw(void) { }

void SpindashDust_Clear(void) {
    u32 i;
    for (i = 0; i < SPINDASHDUST_MAX; i++) g_spindashdusts[i].active = 0;
}

void SpindashDust_Reset(void) { SpindashDust_Clear(); }
