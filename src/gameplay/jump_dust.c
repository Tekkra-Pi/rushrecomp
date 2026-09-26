/* Gameplay level jump dust functions. */
#include "nds_types.h"

#define JUMPDUST_MAX 8

void JumpDust_Init(void) { }

s32 JumpDust_Spawn(s32 x, s32 y) {
    u32 i;
    for (i = 0; i < JUMPDUST_MAX; i++) {
        if (g_dusts[i].active) continue;
        g_dusts[i].x = x;
        g_dusts[i].y = y;
        g_dusts[i].vel_x = (i & 1) ? 0x60 : -0x60;
        g_dusts[i].vel_y = -0x80;
        g_dusts[i].timer = 0;
        g_dusts[i].active = 1;
        return i;
    }
    return -1;
}

void JumpDust_Update(void) {
    u32 i;
    for (i = 0; i < JUMPDUST_MAX; i++) {
        if (!g_dusts[i].active) continue;
        g_dusts[i].x += g_dusts[i].vel_x;
        g_dusts[i].y += g_dusts[i].vel_y;
        g_dusts[i].timer++;
        if (g_dusts[i].timer >= 10) g_dusts[i].active = 0;
    }
}

void JumpDust_Draw(void) { }

void JumpDust_Clear(void) {
    u32 i;
    for (i = 0; i < JUMPDUST_MAX; i++) g_dusts[i].active = 0;
}

void JumpDust_Reset(void) { JumpDust_Clear(); }
