/* Gameplay level dust functions. */
#include "nds_types.h"

#define DUST_MAX 16

void Dust_Init(void) { }

s32 Dust_Spawn(s32 x, s32 y, u32 param) {
    u32 i;
    for (i = 0; i < DUST_MAX; i++) {
        if (g_dusts[i].active) continue;
        g_dusts[i].x = x;
        g_dusts[i].y = y;
        g_dusts[i].vel_x = 0;
        g_dusts[i].vel_y = -0x80;
        g_dusts[i].timer = 0;
        g_dusts[i].active = 1;
        return i;
    }
    return -1;
}

void Dust_Update(void) {
    u32 i;
    for (i = 0; i < DUST_MAX; i++) {
        if (!g_dusts[i].active) continue;
        g_dusts[i].x += g_dusts[i].vel_x;
        g_dusts[i].y += g_dusts[i].vel_y;
        g_dusts[i].timer++;
        if (g_dusts[i].timer >= 20) g_dusts[i].active = 0;
    }
}

void Dust_Draw(void) { }

void Dust_Remove(u32 index) {
    if (index < DUST_MAX) g_dusts[index].active = 0;
}

u32 Dust_GetCount(void) { return DUST_MAX; }

s32 Dust_IsActive(u32 index) {
    if (index >= DUST_MAX) return 0;
    return g_dusts[index].active;
}

void Dust_SpawnAt(void) { }

void Dust_Clear(void) {
    u32 i;
    for (i = 0; i < DUST_MAX; i++) g_dusts[i].active = 0;
}
