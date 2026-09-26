/* Gameplay level moving platform functions. */
#include "nds_types.h"


void MovePlat_Init(void) { g_moveplat_count = 0; }

s32 MovePlat_Add(s32 x, s32 y, s32 w, s32 h, s32 dx, s32 dy, u32 speed) {
    if (g_moveplat_count >= 16) return -1;
    u32 i = g_moveplat_count;
    g_moveplats[i].x = x; g_moveplats[i].y = y;
    g_moveplats[i].w = w; g_moveplats[i].h = h;
    g_moveplats[i].origin_x = x; g_moveplats[i].origin_y = y;
    g_moveplats[i].dx = dx; g_moveplats[i].dy = dy;
    g_moveplats[i].speed = speed; g_moveplats[i].timer = 0;
    g_moveplats[i].carrying = 0; g_moveplats[i].active = 1;
    g_moveplat_count++; return i;
}

void MovePlat_Update(void) {
    u32 i;
    for (i = 0; i < g_moveplat_count; i++) {
        if (!g_moveplats[i].active) continue;
        g_moveplats[i].timer++;
        s32 prev_x = g_moveplats[i].x;
        s32 prev_y = g_moveplats[i].y;
        g_moveplats[i].x = g_moveplats[i].origin_x +
            Math_Sin(g_moveplats[i].timer * g_moveplats[i].speed) * g_moveplats[i].dx;
        g_moveplats[i].y = g_moveplats[i].origin_y +
            Math_Sin(g_moveplats[i].timer * g_moveplats[i].speed) * g_moveplats[i].dy;
        s32 delta_x = g_moveplats[i].x - prev_x;
        s32 delta_y = g_moveplats[i].y - prev_y;
        s32 px, py;
        Player_GetPosition(&px, &py);
        if (px >= g_moveplats[i].x && px <= g_moveplats[i].x + g_moveplats[i].w &&
            py >= g_moveplats[i].y - 0x200 && py <= g_moveplats[i].y + 0x200) {
            Player_SetPosition(px + delta_x, py + delta_y);
        }
    }
}

void MovePlat_Remove(u32 i) { if (i < g_moveplat_count) g_moveplat_count--; }
u32 MovePlat_GetCount(void) { return g_moveplat_count; }
void MovePlat_Clear(void) { g_moveplat_count = 0; }
void MovePlat_Reset(void) { g_moveplat_count = 0; }
