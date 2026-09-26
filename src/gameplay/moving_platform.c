#include "nds_types.h"
static u32 s_count;
void MovingPlatform_Init(void) { s_count = 0; }
s32 MovingPlatform_Add(s32 x, s32 y, s32 w, s32 h, s32 dx, s32 dy, s32 speed) {
    if (s_count >= 16) return -1;
    g_moveplats[s_count].x = x; g_moveplats[s_count].y = y;
    g_moveplats[s_count].w = w; g_moveplats[s_count].h = h;
    g_moveplats[s_count].origin_x = x; g_moveplats[s_count].origin_y = y;
    g_moveplats[s_count].dx = dx; g_moveplats[s_count].dy = dy;
    g_moveplats[s_count].speed = speed; g_moveplats[s_count].timer = 0;
    g_moveplats[s_count].carrying = 0; g_moveplats[s_count].active = 1;
    s_count++; return s_count - 1;
}
void MovingPlatform_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_moveplats[i].active) continue;
        g_moveplats[i].timer++;
        s32 phase = g_moveplats[i].timer * g_moveplats[i].speed / 256;
        if (phase > 256) phase = 256;
        g_moveplats[i].x = g_moveplats[i].origin_x + g_moveplats[i].dx * phase / 256;
        g_moveplats[i].y = g_moveplats[i].origin_y + g_moveplats[i].dy * phase / 256;
    }
}
void MovingPlatform_Remove(u32 index) { if (index < s_count) g_moveplats[index].active = 0; }
u32 MovingPlatform_GetCount(void) { return s_count; }
void MovingPlatform_Clear(void) { s_count = 0; }
void MovingPlatform_Reset(void) { s_count = 0; }
