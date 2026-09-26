/* Gameplay level dust cloud FX functions. */
#include "nds_types.h"

#define DUSTCLOUD_MAX 16

void DustCloud_Init(void) { }

s32 DustCloud_Spawn(s32 x, s32 y, u32 param) {
    u32 i;
    for (i = 0; i < DUSTCLOUD_MAX; i++) {
        if (g_dustclouds[i].active) continue;
        g_dustclouds[i].x = x;
        g_dustclouds[i].y = y;
        g_dustclouds[i].vel_x = (param & 1) ? 0x80 : -0x80;
        g_dustclouds[i].vel_y = -0x40;
        g_dustclouds[i].timer = 0;
        g_dustclouds[i].active = 1;
        return i;
    }
    return -1;
}

void DustCloud_Update(void) {
    u32 i;
    for (i = 0; i < DUSTCLOUD_MAX; i++) {
        if (!g_dustclouds[i].active) continue;
        g_dustclouds[i].x += g_dustclouds[i].vel_x;
        g_dustclouds[i].y += g_dustclouds[i].vel_y;
        g_dustclouds[i].timer++;
        if (g_dustclouds[i].timer >= 24) g_dustclouds[i].active = 0;
    }
}

void DustCloud_Draw(void) { }

u32 DustCloud_GetCount(void) { return DUSTCLOUD_MAX; }

void DustCloud_Clear(void) {
    u32 i;
    for (i = 0; i < DUSTCLOUD_MAX; i++) g_dustclouds[i].active = 0;
}

void DustCloud_Reset(void) { DustCloud_Clear(); }
