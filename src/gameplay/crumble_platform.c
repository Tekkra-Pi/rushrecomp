/* Crumbling platform functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);

#define CRUMBLEPLAT_MAX 16

static void CrumblePlatform_RespawnInternal(void);

void CrumblePlatform_Init(void) { g_crumbleplat_count = 0; }

s32 CrumblePlatform_CheckPlayerOn(void) {
    u32 i;
    for (i = 0; i < g_crumbleplat_count; i++) {
        if (!g_crumbleplats[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        if (px >= g_crumbleplats[i].x && px <= g_crumbleplats[i].x + g_crumbleplats[i].w &&
            py >= g_crumbleplats[i].y - 0x200 && py <= g_crumbleplats[i].y + 0x200) {
            return i;
        }
    }
    return -1;
}

void CrumblePlatform_Shake(void) {
    u32 i;
    for (i = 0; i < g_crumbleplat_count; i++) {
        if (!g_crumbleplats[i].active || g_crumbleplats[i].crumbling) continue;
        g_crumbleplats[i].crumbling = 1;
        g_crumbleplats[i].timer = 0;
    }
}

void CrumblePlatform_Update(void) {
    u32 i;
    for (i = 0; i < g_crumbleplat_count; i++) {
        if (!g_crumbleplats[i].active) continue;
        if (g_crumbleplats[i].crumbling) {
            g_crumbleplats[i].timer++;
            if (g_crumbleplats[i].timer >= 30) {
                g_crumbleplats[i].gone = 1;
                g_crumbleplats[i].crumbling = 0;
                g_crumbleplats[i].active = 0;
                g_crumbleplats[i].respawn_timer = 0;
            }
        } else if (g_crumbleplats[i].gone) {
            g_crumbleplats[i].respawn_timer++;
            if (g_crumbleplats[i].respawn_timer >= 120) {
                CrumblePlatform_RespawnInternal();
            }
        }
    }
}

static void CrumblePlatform_RespawnInternal(void) {
    u32 i;
    for (i = 0; i < g_crumbleplat_count; i++) {
        if (g_crumbleplats[i].gone) {
            g_crumbleplats[i].gone = 0;
            g_crumbleplats[i].active = 1;
            g_crumbleplats[i].timer = 0;
            return;
        }
    }
}

void CrumblePlatform_Respawn(void) {
    CrumblePlatform_RespawnInternal();
}
