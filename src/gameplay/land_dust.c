/* Gameplay level land dust functions. */
#include "nds_types.h"

#define LANDDUST_MAX 8

void LandDust_Init(void) { }

s32 LandDust_Spawn(s32 x, s32 y) {
    u32 i;
    for (i = 0; i < LANDDUST_MAX; i++) {
        if (g_landdusts[i].active) continue;
        g_landdusts[i].x = x;
        g_landdusts[i].y = y;
        g_landdusts[i].vel_x = (i & 1) ? 0x40 : -0x40;
        g_landdusts[i].vel_y = -0x80;
        g_landdusts[i].timer = 0;
        g_landdusts[i].active = 1;
        return i;
    }
    return -1;
}

void LandDust_Update(void) {
    u32 i;
    for (i = 0; i < LANDDUST_MAX; i++) {
        if (!g_landdusts[i].active) continue;
        g_landdusts[i].x += g_landdusts[i].vel_x;
        g_landdusts[i].y += g_landdusts[i].vel_y;
        g_landdusts[i].timer++;
        if (g_landdusts[i].timer >= 12) g_landdusts[i].active = 0;
    }
}

void LandDust_Draw(void) { }

void LandDust_Clear(void) {
    u32 i;
    for (i = 0; i < LANDDUST_MAX; i++) g_landdusts[i].active = 0;
}

void LandDust_Reset(void) { LandDust_Clear(); }
