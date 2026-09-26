/* Gameplay level whirlpool functions. */
#include "nds_types.h"


void Whirlpool_Init(void) { g_whirlpool_count = 0; }

s32 Whirlpool_Add(s32 x, s32 y, s32 radius, s32 strength) {
    if (g_whirlpool_count >= 4) return -1;
    u32 i = g_whirlpool_count;
    g_whirlpools[i].x = x; g_whirlpools[i].y = y;
    g_whirlpools[i].radius = radius; g_whirlpools[i].strength = strength;
    g_whirlpools[i].angle = 0; g_whirlpools[i].active = 1;
    g_whirlpool_count++; return i;
}

void Whirlpool_Update(void) {
    u32 i;
    for (i = 0; i < g_whirlpool_count; i++) {
        if (!g_whirlpools[i].active) continue;
        g_whirlpools[i].angle += 16;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_whirlpools[i].x;
        s32 dy = py - g_whirlpools[i].y;
        s32 dist = Math_Sqrt(dx * dx + dy * dy);
        if (dist < g_whirlpools[i].radius) {
            s32 pull = g_whirlpools[i].strength * (g_whirlpools[i].radius - dist) / g_whirlpools[i].radius;
            Player_SetVelocity(
                Player_GetVX() + Math_Sin(g_whirlpools[i].angle) * pull,
                Player_GetVY() - pull
            );
        }
    }
}

void Whirlpool_Remove(u32 i) { if (i < g_whirlpool_count) g_whirlpool_count--; }
u32 Whirlpool_GetCount(void) { return g_whirlpool_count; }
void Whirlpool_Clear(void) { g_whirlpool_count = 0; }
void Whirlpool_Reset(void) { g_whirlpool_count = 0; }
