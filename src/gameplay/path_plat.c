/* Gameplay level path platform functions. */
#include "nds_types.h"


void PathPlat_Init(void) { g_pathplat_count = 0; }

s32 PathPlat_Add(s32 x, s32 y, s32 w, s32 h, s32 path[][2], u32 path_len, u32 speed) {
    if (g_pathplat_count >= 8) return -1;
    u32 i = g_pathplat_count;
    g_pathplats[i].x = x; g_pathplats[i].y = y;
    g_pathplats[i].w = w; g_pathplats[i].h = h;
    g_pathplats[i].origin_x = x; g_pathplats[i].origin_y = y;
    g_pathplats[i].path = path;
    g_pathplats[i].path_len = path_len;
    g_pathplats[i].path_idx = 0; g_pathplats[i].timer = 0;
    g_pathplats[i].speed = speed; g_pathplats[i].active = 1;
    g_pathplat_count++; return i;
}

void PathPlat_Update(void) {
    u32 i;
    for (i = 0; i < g_pathplat_count; i++) {
        if (!g_pathplats[i].active || !g_pathplats[i].path) continue;
        g_pathplats[i].timer++;
        if (g_pathplats[i].timer >= g_pathplats[i].speed) {
            g_pathplats[i].timer = 0;
            g_pathplats[i].path_idx = (g_pathplats[i].path_idx + 1) % g_pathplats[i].path_len;
        }
        u32 idx = g_pathplats[i].path_idx;
        s32 target_x = g_pathplats[i].path[idx][0];
        s32 target_y = g_pathplats[i].path[idx][1];
        s32 dx = target_x - g_pathplats[i].x;
        s32 dy = target_y - g_pathplats[i].y;
        s32 prev_x = g_pathplats[i].x;
        g_pathplats[i].x += dx / 4;
        g_pathplats[i].y += dy / 4;
        s32 px, py;
        Player_GetPosition(&px, &py);
        if (px >= g_pathplats[i].x && px <= g_pathplats[i].x + g_pathplats[i].w &&
            py >= g_pathplats[i].y - 0x200 && py <= g_pathplats[i].y + 0x200) {
            Player_SetPosition(px + (g_pathplats[i].x - prev_x), py);
        }
    }
}

void PathPlat_Remove(u32 i) { if (i < g_pathplat_count) g_pathplat_count--; }
u32 PathPlat_GetCount(void) { return g_pathplat_count; }
void PathPlat_Clear(void) { g_pathplat_count = 0; }
void PathPlat_Reset(void) { g_pathplat_count = 0; }
