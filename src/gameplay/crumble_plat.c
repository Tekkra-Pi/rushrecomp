/* Gameplay level crumbling platform functions. */
#include "nds_types.h"


void CrumblePlat_Init(void) { g_crumbleplat_count = 0; }

s32 CrumblePlat_Add(s32 x, s32 y, s32 w, s32 h) {
    if (g_crumbleplat_count >= 16) return -1;
    u32 i = g_crumbleplat_count;
    g_crumbleplats[i].x = x; g_crumbleplats[i].y = y;
    g_crumbleplats[i].w = w; g_crumbleplats[i].h = h;
    g_crumbleplats[i].timer = 0; g_crumbleplats[i].crumbling = 0;
    g_crumbleplats[i].gone = 0; g_crumbleplats[i].respawn_timer = 0;
    g_crumbleplats[i].active = 1;
    g_crumbleplat_count++; return i;
}

void CrumblePlat_Update(void) {
    u32 i;
    for (i = 0; i < g_crumbleplat_count; i++) {
        if (!g_crumbleplats[i].active) continue;
        if (g_crumbleplats[i].gone) {
            g_crumbleplats[i].respawn_timer++;
            if (g_crumbleplats[i].respawn_timer >= 120) {
                g_crumbleplats[i].gone = 0;
                g_crumbleplats[i].crumbling = 0;
                g_crumbleplats[i].timer = 0;
                g_crumbleplats[i].respawn_timer = 0;
            }
            continue;
        }
        s32 px, py;
        Player_GetPosition(&px, &py);
        if (px >= g_crumbleplats[i].x && px <= g_crumbleplats[i].x + g_crumbleplats[i].w &&
            py >= g_crumbleplats[i].y - 0x200 && py <= g_crumbleplats[i].y) {
            if (!g_crumbleplats[i].crumbling) {
                g_crumbleplats[i].crumbling = 1;
            }
        }
        if (g_crumbleplats[i].crumbling) {
            g_crumbleplats[i].timer++;
            if (g_crumbleplats[i].timer >= 30) {
                g_crumbleplats[i].gone = 1;
                Debris_Spawn(g_crumbleplats[i].x, g_crumbleplats[i].y, 3);
            }
        }
    }
}

void CrumblePlat_Remove(u32 i) { if (i < g_crumbleplat_count) g_crumbleplat_count--; }
u32 CrumblePlat_GetCount(void) { return g_crumbleplat_count; }
void CrumblePlat_Clear(void) { g_crumbleplat_count = 0; }
void CrumblePlat_Reset(void) { g_crumbleplat_count = 0; }
