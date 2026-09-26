/* Gameplay level gravity zone functions. */
#include "nds_types.h"


void GravZone_Init(void) { g_gravzone_count = 0; }

s32 GravZone_Add(s32 x, s32 y, s32 w, s32 h, s32 gravity) {
    if (g_gravzone_count >= 8) return -1;
    u32 i = g_gravzone_count;
    g_gravzones[i].x = x; g_gravzones[i].y = y;
    g_gravzones[i].w = w; g_gravzones[i].h = h;
    g_gravzones[i].gravity = gravity; g_gravzones[i].active = 1;
    g_gravzone_count++; return i;
}

void GravZone_Update(void) {
    u32 i;
    for (i = 0; i < g_gravzone_count; i++) {
        if (!g_gravzones[i].active) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        if (px >= g_gravzones[i].x && px <= g_gravzones[i].x + g_gravzones[i].w &&
            py >= g_gravzones[i].y && py <= g_gravzones[i].y + g_gravzones[i].h) {
            Player_SetGravity(g_gravzones[i].gravity);
        }
    }
}

void GravZone_Remove(u32 i) { if (i < g_gravzone_count) g_gravzone_count--; }
u32 GravZone_GetCount(void) { return g_gravzone_count; }
void GravZone_Clear(void) { g_gravzone_count = 0; }
void GravZone_Reset(void) { g_gravzone_count = 0; }
