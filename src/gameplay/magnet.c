/* Gameplay level magnet functions. */
#include "nds_types.h"


void Magnet_Init(void) { g_magnet_count = 0; }

s32 Magnet_Add(s32 x, s32 y, u32 range, u32 strength) {
    if (g_magnet_count >= 4) return -1;
    u32 i = g_magnet_count;
    g_magnets[i].x = x; g_magnets[i].y = y;
    g_magnets[i].range = range; g_magnets[i].strength = strength;
    g_magnets[i].active = 1;
    g_magnet_count++; return i;
}

void Magnet_Update(void) {
    u32 i;
    for (i = 0; i < g_magnet_count; i++) {
        if (!g_magnets[i].active) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = g_magnets[i].x - px;
        s32 dy = g_magnets[i].y - py;
        if (dx < 0) dx = -dx;
        if (dy < 0) dy = -dy;
        if (dx < (s32)g_magnets[i].range && dy < (s32)g_magnets[i].range) {
            Player_AttractRings(g_magnets[i].x, g_magnets[i].y, g_magnets[i].strength);
        }
    }
}

void Magnet_Remove(u32 i) { if (i < g_magnet_count) g_magnet_count--; }
u32 Magnet_GetCount(void) { return g_magnet_count; }
void Magnet_Clear(void) { g_magnet_count = 0; }
void Magnet_Reset(void) { g_magnet_count = 0; }
