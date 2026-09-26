/* Gameplay level lava functions. */
#include "nds_types.h"


void Lava_Init(void) { g_lava_count = 0; }

s32 Lava_Add(s32 x, s32 y, s32 w, s32 h, u32 speed) {
    if (g_lava_count >= 4) return -1;
    u32 i = g_lava_count;
    g_lavas[i].x = x; g_lavas[i].y = y;
    g_lavas[i].w = w; g_lavas[i].h = h;
    g_lavas[i].speed = speed; g_lavas[i].timer = 0;
    g_lavas[i].active = 1;
    g_lava_count++; return i;
}

void Lava_Update(void) {
    u32 i;
    for (i = 0; i < g_lava_count; i++) {
        if (!g_lavas[i].active) continue;
        g_lavas[i].timer++;
        g_lavas[i].y += Math_Sin(g_lavas[i].timer * g_lavas[i].speed) / 256;
        s32 px, py;
        Player_GetPosition(&px, &py);
        if (px >= g_lavas[i].x && px <= g_lavas[i].x + g_lavas[i].w &&
            py >= g_lavas[i].y && py <= g_lavas[i].y + g_lavas[i].h && !Player_IsInvincible()) {
            Player_Hurt();
        }
    }
}

void Lava_Remove(u32 i) { if (i < g_lava_count) g_lava_count--; }
u32 Lava_GetCount(void) { return g_lava_count; }
void Lava_Clear(void) { g_lava_count = 0; }
void Lava_Reset(void) { g_lava_count = 0; }
