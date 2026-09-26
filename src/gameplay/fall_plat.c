/* Gameplay level falling platform functions. */
#include "nds_types.h"


void FallPlat_Init(void) { g_fallplat_count = 0; }

s32 FallPlat_Add(s32 x, s32 y, s32 w, s32 h) {
    if (g_fallplat_count >= 16) return -1;
    u32 i = g_fallplat_count;
    g_fallplats[i].x = x; g_fallplats[i].y = y;
    g_fallplats[i].w = w; g_fallplats[i].h = h;
    g_fallplats[i].origin_y = y;
    g_fallplats[i].timer = 0; g_fallplats[i].falling = 0;
    g_fallplats[i].fallen = 0; g_fallplats[i].fall_vy = 0;
    g_fallplats[i].active = 1;
    g_fallplat_count++; return i;
}

void FallPlat_Update(void) {
    u32 i;
    for (i = 0; i < g_fallplat_count; i++) {
        if (!g_fallplats[i].active || g_fallplats[i].fallen) continue;
        if (!g_fallplats[i].falling) {
            s32 px, py;
            Player_GetPosition(&px, &py);
            if (px >= g_fallplats[i].x && px <= g_fallplats[i].x + g_fallplats[i].w &&
                py >= g_fallplats[i].y - 0x200 && py <= g_fallplats[i].y) {
                g_fallplats[i].falling = 1;
            }
        }
        if (g_fallplats[i].falling) {
            g_fallplats[i].timer++;
            if (g_fallplats[i].timer >= 15) {
                g_fallplats[i].fall_vy += 0x40;
                g_fallplats[i].y += g_fallplats[i].fall_vy;
                if (g_fallplats[i].y > g_fallplats[i].origin_y + 0x2000) {
                    g_fallplats[i].fallen = 1;
                }
            }
        }
    }
}

void FallPlat_Remove(u32 i) { if (i < g_fallplat_count) g_fallplat_count--; }
u32 FallPlat_GetCount(void) { return g_fallplat_count; }
void FallPlat_Clear(void) { g_fallplat_count = 0; }
void FallPlat_Reset(void) { g_fallplat_count = 0; }
